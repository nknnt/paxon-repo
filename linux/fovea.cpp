#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <sys/ioctl.h>
#include <unistd.h>

struct TermSize {
    int cols;
    int rows;
};

static TermSize get_terminal_size() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0 && w.ws_row > 0) {
        return {w.ws_col, w.ws_row};
    }
    return {80, 24}; // fallback
}

struct RGB {
    unsigned char r, g, b;
};

// ---------------------------------------------------------------------
// 감마 보정: sRGB(감마 인코딩) <-> linear light 변환.
// box filter로 여러 픽셀을 평균낼 때 감마 인코딩된 값을 그대로 더하면
// 실제 빛의 평균과 달라져서 중간톤이 어둡게 뭉개짐. linear 공간에서
// 평균을 낸 뒤 다시 sRGB로 되돌리면 훨씬 자연스러운 색이 나옴.
// ---------------------------------------------------------------------
static double g_srgb_to_linear[256];

static void init_gamma_table() {
    for (int i = 0; i < 256; i++) {
        double c = i / 255.0;
        g_srgb_to_linear[i] = (c <= 0.04045) ? (c / 12.92)
                                              : std::pow((c + 0.055) / 1.055, 2.4);
    }
}

static inline unsigned char linear_to_srgb(double v) {
    if (v <= 0.0) return 0;
    if (v >= 1.0) return 255;
    double s = (v <= 0.0031308) ? (v * 12.92)
                                 : (1.055 * std::pow(v, 1.0 / 2.4) - 0.055);
    int r = (int)(s * 255.0 + 0.5);
    if (r < 0) r = 0;
    if (r > 255) r = 255;
    return (unsigned char)r;
}

// box filter: [u0,u1) x [v0,v1) 영역(정규화 좌표) 내 픽셀을 linear light
// 공간에서 평균낸 뒤 sRGB로 변환해 반환.
static RGB sample_box_gamma(const unsigned char* data, int width, int height, int channels,
                             double u0, double u1, double v0, double v1) {
    int x0 = (int)(u0 * width);
    int x1 = (int)(u1 * width);
    int y0 = (int)(v0 * height);
    int y1 = (int)(v1 * height);

    if (x1 <= x0) x1 = x0 + 1;
    if (y1 <= y0) y1 = y0 + 1;
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 > width)  x1 = width;
    if (y1 > height) y1 = height;
    if (x0 >= x1) x0 = x1 - 1;
    if (y0 >= y1) y0 = y1 - 1;

    double sum_r = 0.0, sum_g = 0.0, sum_b = 0.0;
    long count = 0;

    for (int y = y0; y < y1; y++) {
        const unsigned char* row = data + (size_t)y * width * channels;
        for (int x = x0; x < x1; x++) {
            const unsigned char* p = row + (size_t)x * channels;
            unsigned char r = p[0];
            unsigned char g = (channels >= 2) ? p[1] : p[0];
            unsigned char b = (channels >= 3) ? p[2] : p[0];
            sum_r += g_srgb_to_linear[r];
            sum_g += g_srgb_to_linear[g];
            sum_b += g_srgb_to_linear[b];
            count++;
        }
    }

    RGB c;
    c.r = linear_to_srgb(sum_r / count);
    c.g = linear_to_srgb(sum_g / count);
    c.b = linear_to_srgb(sum_b / count);
    return c;
}

// ---------------------------------------------------------------------
// Unicode Sextant 블록 (U+1FB00 ~ U+1FB3B): 문자 셀 하나를 2열x3행,
// 즉 6개의 서브픽셀로 나눠 표현. half-block(▄) 대비 셀당 3배 해상도.
// 유니코드 공식 차트(unicode.org/charts/nameslist/n_1FB00.html) 순서 그대로.
// 셀 레이아웃:
//   1 2
//   3 4
//   5 6
// ---------------------------------------------------------------------
static const char* SEXTANT_NAMES[60] = {
    "1","2","12","3","13","23","123","4","14","24","124","34","134","234","1234",
    "5","15","25","125","35","235","1235","45","145","245","1245","345","1345","2345","12345",
    "6","16","26","126","36","136","236","1236","46","146","1246","346","1346","2346","12346",
    "56","156","256","1256","356","1356","2356","12356","456","1456","2456","12456","3456","13456","23456"
};

static std::unordered_map<int, uint32_t> g_mask_to_cp;

static void init_sextant_table() {
    for (int i = 0; i < 60; i++) {
        int mask = 0;
        for (const char* p = SEXTANT_NAMES[i]; *p; ++p) {
            int pos = *p - '0'; // 1..6
            mask |= (1 << (pos - 1));
        }
        g_mask_to_cp[mask] = 0x1FB00 + i;
    }
    g_mask_to_cp[0]    = 0x20;    // 전부 off -> 스페이스
    g_mask_to_cp[0x3F] = 0x2588;  // 전부 on  -> full block
    // 왼쪽 세로줄(1,3,5)/오른쪽 세로줄(2,4,6) 조합은 이미 Block Elements의
    // LEFT/RIGHT HALF BLOCK과 겹쳐서 sextant 블록에는 별도 글리프가 없음.
    g_mask_to_cp[0b010101] = 0x258C; // positions 1,3,5 -> LEFT HALF BLOCK
    g_mask_to_cp[0b101010] = 0x2590; // positions 2,4,6 -> RIGHT HALF BLOCK
}

static std::string utf8_encode(uint32_t cp) {
    std::string s;
    if (cp <= 0x7F) {
        s += (char)cp;
    } else if (cp <= 0x7FF) {
        s += (char)(0xC0 | (cp >> 6));
        s += (char)(0x80 | (cp & 0x3F));
    } else if (cp <= 0xFFFF) {
        s += (char)(0xE0 | (cp >> 12));
        s += (char)(0x80 | ((cp >> 6) & 0x3F));
        s += (char)(0x80 | (cp & 0x3F));
    } else {
        s += (char)(0xF0 | (cp >> 18));
        s += (char)(0x80 | ((cp >> 12) & 0x3F));
        s += (char)(0x80 | ((cp >> 6) & 0x3F));
        s += (char)(0x80 | (cp & 0x3F));
    }
    return s;
}

static inline double luminance(const RGB& c) {
    return 0.2126 * c.r + 0.7152 * c.g + 0.0722 * c.b;
}

#define FOVEA_VERSION "0.1.1"

static void print_usage(const char* prog) {
    fprintf(stderr,
        "usage: %s <image_path> [options]\n"
        "\n"
        "options:\n"
        "  --width N     force output width in columns (capped to terminal width)\n"
        "  -h, --help    show this help message and exit\n"
        "  -v, --version show version information and exit\n",
        prog);
}

static void print_version() {
    printf("fovea %s\n", FOVEA_VERSION);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    // 먼저 -h/-v 같은 플래그를 스캔 (이미지 경로 없이도 동작해야 함)
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            print_version();
            return 0;
        }
    }

    std::string path = argv[1];
    int forced_width = 0;

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--width") == 0 && i + 1 < argc) {
            forced_width = atoi(argv[i + 1]);
            i++;
        }
    }

    init_gamma_table();
    init_sextant_table();

    int img_w, img_h, img_channels;
    unsigned char* data = stbi_load(path.c_str(), &img_w, &img_h, &img_channels, 0);
    if (!data) {
        fprintf(stderr, "fovea: could not load image: %s\n", path.c_str());
        return 1;
    }

    TermSize term = get_terminal_size();

    int out_cols = (forced_width > 0) ? forced_width : term.cols;
    if (out_cols > term.cols) out_cols = term.cols;
    if (out_cols < 1) out_cols = 1;

    // 문자 셀 가로:세로 ~= 1:2 비율 보정 (기존과 동일한 전체 셀 그리드)
    double aspect = (double)img_h / (double)img_w;
    int out_lines = (int)(out_cols * aspect * 0.5);
    if (out_lines < 1) out_lines = 1;

    std::string out;
    out.reserve((size_t)out_cols * out_lines * 40);

    char buf[64];
    RGB sub[6]; // 셀 하나의 6개 서브픽셀: 1,2 / 3,4 / 5,6 순서

    for (int line = 0; line < out_lines; line++) {
        double v0 = (double)line / out_lines;
        double v1 = (double)(line + 1) / out_lines;

        for (int col = 0; col < out_cols; col++) {
            double u0 = (double)col / out_cols;
            double u1 = (double)(col + 1) / out_cols;

            for (int subrow = 0; subrow < 3; subrow++) {
                double sv0 = v0 + (v1 - v0) * subrow / 3.0;
                double sv1 = v0 + (v1 - v0) * (subrow + 1) / 3.0;
                for (int subcol = 0; subcol < 2; subcol++) {
                    double su0 = u0 + (u1 - u0) * subcol / 2.0;
                    double su1 = u0 + (u1 - u0) * (subcol + 1) / 2.0;
                    sub[subrow * 2 + subcol] =
                        sample_box_gamma(data, img_w, img_h, img_channels, su0, su1, sv0, sv1);
                }
            }

            double lum[6];
            double mean_lum = 0.0;
            for (int i = 0; i < 6; i++) {
                lum[i] = luminance(sub[i]);
                mean_lum += lum[i];
            }
            mean_lum /= 6.0;

            int mask = 0;
            long fg_r = 0, fg_g = 0, fg_b = 0, fg_n = 0;
            long bg_r = 0, bg_g = 0, bg_b = 0, bg_n = 0;

            for (int i = 0; i < 6; i++) {
                if (lum[i] >= mean_lum) {
                    mask |= (1 << i);
                    fg_r += sub[i].r; fg_g += sub[i].g; fg_b += sub[i].b; fg_n++;
                } else {
                    bg_r += sub[i].r; bg_g += sub[i].g; bg_b += sub[i].b; bg_n++;
                }
            }

            RGB fg{0, 0, 0}, bg{0, 0, 0};
            if (fg_n > 0) {
                fg = { (unsigned char)(fg_r / fg_n), (unsigned char)(fg_g / fg_n), (unsigned char)(fg_b / fg_n) };
            }
            if (bg_n > 0) {
                bg = { (unsigned char)(bg_r / bg_n), (unsigned char)(bg_g / bg_n), (unsigned char)(bg_b / bg_n) };
            }
            if (fg_n == 0) fg = bg; // 전부 off (space) -> fg는 안 쓰이지만 안전하게 채움
            if (bg_n == 0) bg = fg; // 전부 on (full block) -> bg는 안 쓰이지만 안전하게 채움

            uint32_t cp = g_mask_to_cp[mask];
            std::string glyph = utf8_encode(cp);

            snprintf(buf, sizeof(buf), "\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm",
                     fg.r, fg.g, fg.b, bg.r, bg.g, bg.b);
            out += buf;
            out += glyph;
        }
        out += "\x1b[0m\n";
    }

    fwrite(out.data(), 1, out.size(), stdout);

    stbi_image_free(data);
    return 0;
}