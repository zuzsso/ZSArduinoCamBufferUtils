#include <zs_camera_buffer_utils.h>
#include <esp_camera.h>
#include <zs_serial_port_utils.h>
#include <zs_imageometry.h>
#include <stdio.h>

namespace ZS
{
    namespace CameraBufferUtils
    {
        const uint8_t font5x7[95][5] = {
            // ASCII 32 to 126
            // Space to ~
            {0x00, 0x00, 0x00, 0x00, 0x00}, // ' '
            {0x00, 0x00, 0x5F, 0x00, 0x00}, // !
            {0x00, 0x07, 0x00, 0x07, 0x00}, // "
            {0x14, 0x7F, 0x14, 0x7F, 0x14}, // #
            {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $
            {0x23, 0x13, 0x08, 0x64, 0x62}, // %
            {0x36, 0x49, 0x55, 0x22, 0x50}, // &
            {0x00, 0x05, 0x03, 0x00, 0x00}, // '
            {0x00, 0x1C, 0x22, 0x41, 0x00}, // (
            {0x00, 0x41, 0x22, 0x1C, 0x00}, // )
            {0x14, 0x08, 0x3E, 0x08, 0x14}, // *
            {0x08, 0x08, 0x3E, 0x08, 0x08}, // +
            {0x00, 0x50, 0x30, 0x00, 0x00}, // ,
            {0x08, 0x08, 0x08, 0x08, 0x08}, // -
            {0x00, 0x60, 0x60, 0x00, 0x00}, // .
            {0x03, 0x04, 0x08, 0x10, 0x60}, // /

            {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
            {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
            {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
            {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
            {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
            {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
            {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
            {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
            {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
            {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9

            {0x00, 0x36, 0x36, 0x00, 0x00}, // :
            {0x00, 0x56, 0x36, 0x00, 0x00}, // ;
            {0x08, 0x14, 0x22, 0x41, 0x00}, // <
            {0x14, 0x14, 0x14, 0x14, 0x14}, // =
            {0x00, 0x41, 0x22, 0x14, 0x08}, // >
            {0x02, 0x01, 0x51, 0x09, 0x06}, // ?

            {0x3E, 0x41, 0x5D, 0x55, 0x1E}, // @
            {0x7C, 0x12, 0x11, 0x12, 0x7C}, // A
            {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
            {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
            {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
            {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
            {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
            {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
            {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
            {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
            {0x20, 0x40, 0x41, 0x3F, 0x01}, // J
            {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
            {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
            {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
            {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
            {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O

            {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
            {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
            {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
            {0x46, 0x49, 0x49, 0x49, 0x31}, // S
            {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
            {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
            {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
            {0x7F, 0x20, 0x18, 0x20, 0x7F}, // W
            {0x63, 0x14, 0x08, 0x14, 0x63}, // X
            {0x07, 0x08, 0x70, 0x08, 0x07}, // Y
            {0x61, 0x51, 0x49, 0x45, 0x43}, // Z

            {0x00, 0x7F, 0x41, 0x41, 0x00}, // [
            {0x60, 0x10, 0x08, 0x04, 0x03}, // backslash
            {0x00, 0x41, 0x41, 0x7F, 0x00}, // ]
            {0x04, 0x02, 0x01, 0x02, 0x04}, // ^
            {0x40, 0x40, 0x40, 0x40, 0x40}, // _

            {0x00, 0x01, 0x02, 0x04, 0x00}, // `
            {0x20, 0x54, 0x54, 0x54, 0x78}, // a
            {0x7F, 0x48, 0x44, 0x44, 0x38}, // b
            {0x38, 0x44, 0x44, 0x44, 0x20}, // c
            {0x38, 0x44, 0x44, 0x48, 0x7F}, // d
            {0x38, 0x54, 0x54, 0x54, 0x18}, // e
            {0x08, 0x7E, 0x09, 0x01, 0x02}, // f
            {0x08, 0x54, 0x54, 0x54, 0x3C}, // g
            {0x7F, 0x08, 0x04, 0x04, 0x78}, // h
            {0x00, 0x44, 0x7D, 0x40, 0x00}, // i
            {0x20, 0x40, 0x44, 0x3D, 0x00}, // j
            {0x7F, 0x10, 0x28, 0x44, 0x00}, // k
            {0x00, 0x41, 0x7F, 0x40, 0x00}, // l
            {0x7C, 0x04, 0x18, 0x04, 0x78}, // m
            {0x7C, 0x08, 0x04, 0x04, 0x78}, // n
            {0x38, 0x44, 0x44, 0x44, 0x38}, // o

            {0x7C, 0x14, 0x14, 0x14, 0x08}, // p
            {0x08, 0x14, 0x14, 0x18, 0x7C}, // q
            {0x7C, 0x08, 0x04, 0x04, 0x08}, // r
            {0x48, 0x54, 0x54, 0x54, 0x20}, // s
            {0x04, 0x3F, 0x44, 0x40, 0x20}, // t
            {0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
            {0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
            {0x3C, 0x40, 0x30, 0x40, 0x3C}, // w
            {0x44, 0x28, 0x10, 0x28, 0x44}, // x
            {0x0C, 0x50, 0x50, 0x50, 0x3C}, // y
            {0x44, 0x64, 0x54, 0x4C, 0x44}, // z

            {0x08, 0x36, 0x41, 0x41, 0x00}, // {
            {0x00, 0x00, 0x7F, 0x00, 0x00}, // |
            {0x00, 0x41, 0x41, 0x36, 0x08}, // }
            {0x02, 0x01, 0x02, 0x04, 0x02}, // ~
        };

        const uint8_t *get5x7Bitmap(char c)
        {
            if (c < 32 || c > 126)
                c = '?'; // Fallback to '?'
            return font5x7[c - 32];
        }

        void checkImagePreconditions(camera_fb_t *frame)
        {
            if (!frame)
            {
                ZS::SerialPort::runtimeException("No frame to draw on");
            }

            if (frame->format != PIXFORMAT_RGB565)
            {
                ZS::SerialPort::runtimeException("Image pixel format not PIXFORMAT_RGB565");
            }
        }

        uint8_t convertRgb565ToGrayscaleIndex(uint16_t rgb565)
        {
            // Extract RGB components
            uint8_t r5 = (rgb565 >> 11) & 0x1F;
            uint8_t g6 = (rgb565 >> 5) & 0x3F;
            uint8_t b5 = rgb565 & 0x1F;

            // Convert to 8-bit per channel
            uint8_t r = (r5 * 255) / 31;
            uint8_t g = (g6 * 255) / 63;
            uint8_t b = (b5 * 255) / 31;

            // Weighted average
            return (uint8_t)((r * 30 + g * 59 + b * 11) / 100); // Equivalent to 0.299r + 0.587g + 0.114b
        }

        void drawPixel(camera_fb_t *fb, int x, int y, uint16_t coloRgb565)
        {
            checkImagePreconditions(fb);
            if (x >= fb->width || y >= fb->height)
                return;
            if (fb->format == PIXFORMAT_RGB565)
            {
                uint16_t *buf = (uint16_t *)fb->buf;
                buf[y * fb->width + x] = coloRgb565;
            }
            else if (fb->format == PIXFORMAT_GRAYSCALE)
            {
                fb->buf[y * fb->width + x] = convertRgb565ToGrayscaleIndex(coloRgb565);
            }
        }

        void drawChar5x7(camera_fb_t *fb, char c, int x, int y, uint16_t color_rgb565)
        {
            checkImagePreconditions(fb);

            const uint8_t *bitmap = get5x7Bitmap(c);
            if (!bitmap)
                return;

            for (int col = 0; col < 5; col++)
            {
                uint8_t col_data = bitmap[col];
                for (int row = 0; row < 7; row++)
                {
                    if (col_data & (1 << row))
                    {
                        drawPixel(fb, x + col, y + row, color_rgb565);
                    }
                }
            }
        }

        void draw7x5Text(ZS::ImaGeometry::ImageCoordinate *coordinate, const char *text, camera_fb_t *frame, uint16_t color)
        {
            checkImagePreconditions(frame);
            int cursorX = coordinate->getX();
            int cursorY = coordinate->getY(); 
            while (*text)
            {
                drawChar5x7(frame, *text, cursorX, cursorY, color);
                cursorX += 6; // 5 pixels + 1 space
                text++;
            }
        }

        void drawLine(ZS::ImaGeometry::ImageLine *line, camera_fb_t *frame, uint16_t color)
        {
            checkImagePreconditions(frame);

            int x0 = line->getStart().getX();
            int y0 = line->getStart().getY();

            int x1 = line->getEnd().getX();
            int y1 = line->getEnd().getY();

            int dx = abs(x1 - x0);
            int dy = -abs(y1 - y0);
            int sx = (x0 < x1) ? 1 : -1;
            int sy = (y0 < y1) ? 1 : -1;
            int err = dx + dy;

            while (true)
            {
                if (x0 >= 0 && x0 < frame->width && y0 >= 0 && y0 < frame->height)
                {
                    size_t index = (y0 * frame->width + x0) * 2;
                    frame->buf[index] = color >> 8;
                    frame->buf[index + 1] = color & 0xFF;
                }

                if (x0 == x1 && y0 == y1)
                    break;
                int e2 = 2 * err;
                if (e2 >= dy)
                {
                    err += dy;
                    x0 += sx;
                }
                if (e2 <= dx)
                {
                    err += dx;
                    y0 += sy;
                }
            }
        }

        void drawCircle(ZS::ImaGeometry::Circle *circle, camera_fb_t *frame, uint16_t color)
        {
            checkImagePreconditions(frame);

            int x = 0, y = circle->getRadiusPx();
            int d = 3 - 2 * circle->getRadiusPx();
            int xc = circle->getCenter().getX();
            int yc = circle->getCenter().getY();

            while (y >= x)
            {
                int points[8][2] = {
                    {xc + x, yc + y},
                    {xc - x, yc + y},
                    {xc + x, yc - y},
                    {xc - x, yc - y},
                    {xc + y, yc + x},
                    {xc - y, yc + x},
                    {xc + y, yc - x},
                    {xc - y, yc - x}};

                for (int i = 0; i < 8; i++)
                {
                    int px = points[i][0];
                    int py = points[i][1];
                    if (px >= 0 && px < frame->width && py >= 0 && py < frame->height)
                    {
                        size_t index = (py * frame->width + px) * 2;
                        frame->buf[index] = color >> 8;
                        frame->buf[index + 1] = color & 0xFF;
                    }
                }

                x++;
                if (d > 0)
                {
                    y--;
                    d = d + 4 * (x - y) + 10;
                }
                else
                {
                    d = d + 4 * x + 6;
                }
            }
        }

        void drawRectangle(ZS::ImaGeometry::Rectangle *rectangle, camera_fb_t *frame, uint16_t color)
        {
            checkImagePreconditions(frame);

            std::vector<ZS::ImaGeometry::ImageLine> edges = rectangle->getEdges();

            for (ZS::ImaGeometry::ImageLine edge : edges)
            {
                drawLine(&edge, frame, color);
            }
        }

        void draw5x3Char(camera_fb_t *frame, ZS::ImaGeometry::ImageCoordinate coord, char c, uint16_t color)
        {
            checkImagePreconditions(frame);

            int x = coord.getX();
            int y = coord.getY();

            const uint8_t font5x7[][5] = {
                {0x7C, 0xA2, 0x92, 0x8A, 0x7C}, // '0'
                {0x00, 0x84, 0xFE, 0x80, 0x00}, // '1'
                {0xC4, 0xA2, 0x92, 0x92, 0x8C}, // '2'
                {0x44, 0x82, 0x92, 0x92, 0x6C}, // '3'
                {0x30, 0x28, 0x24, 0xFE, 0x20}, // '4'
                {0x5E, 0x92, 0x92, 0x92, 0x62}, // '5'
                {0x7C, 0x92, 0x92, 0x92, 0x64}, // '6'
                {0x02, 0xE2, 0x12, 0x0A, 0x06}, // '7'
                {0x6C, 0x92, 0x92, 0x92, 0x6C}, // '8'
                {0x4C, 0x92, 0x92, 0x92, 0x7C}, // '9'
            };

            if (c < '0' || c > '9')
            {
                ZS::SerialPort::runtimeException("Can't write string to image. Only characters 0-9 supported");
            }

            const uint8_t *bitmap = font5x7[c - '0'];

            for (int col = 0; col < 5; col++)
            {
                uint8_t line = bitmap[col];
                for (int row = 0; row < 8; row++)
                {
                    if (line & (1 << row))
                    { // each bit is a row
                        int px = x + col;
                        int py = y + row;
                        if (px >= 0 && px < frame->width && py >= 0 && py < frame->height)
                        {
                            size_t index = (py * frame->width + px) * 2;
                            frame->buf[index] = color >> 8;
                            frame->buf[index + 1] = color & 0xFF;
                        }
                    }
                }
            }
        }

        void draw5x3Text(ZS::ImaGeometry::ImageCoordinate *insertionPoint, const char *text, camera_fb_t *frame, uint16_t color)
        {
            checkImagePreconditions(frame);

            int x = insertionPoint->getX();

            while (*text)
            {
                draw5x3Char(frame, ZS::ImaGeometry::ImageCoordinate(x, insertionPoint->getY()), *text, color);
                x += 6; // 5px wide + 1px spacing
                text++;
            }
        }

        void drawConvexArea(ZS::ImaGeometry::ConvexArea *area, camera_fb_t *frame, uint16_t color)
        {
            checkImagePreconditions(frame);

            std::vector<ZS::ImaGeometry::ImageCoordinate> vertices = area->getVertices();

            int n = vertices.size();
            for (int i = 0; i < n; i++)
            {
                ZS::ImaGeometry::ImageLine il = ZS::ImaGeometry::ImageLine(vertices[i], vertices[(i + 1) % n]);
                drawLine(&il, frame, color);
            }
        }

        uint8_t getGrayscaleIndex(camera_fb_t *frame, int x, int y)
        {

            uint8_t *data = frame->buf;
            int width = frame->width;
            int height = frame->height;

            char buffer[100];

            if (!((x >= 0) && (x < width)))
            {
                sprintf(buffer, "X coordinate out of bounds. Given: %d. Available:[0, %d]", x, (width - 1));
                ZS::SerialPort::runtimeException(buffer);
            }

            if (!((y >= 0) && (y < height)))
            {
                sprintf(buffer, "Y coordinate out of bounds. Given: %d. Avaiable:[0, %d]", y, (height - 1));
                ZS::SerialPort::runtimeException(buffer);
            }

            if (frame->format == PIXFORMAT_RGB565)
            {
                int index = ((y * width) + x) * 2;

                return convertRgb565ToGrayscaleIndex(index);
                // uint8_t high = data[index + 1];
                // uint8_t low = data[index];
                // uint16_t color = (high << 8) | low;
                // int r = ((color >> 11) & 0x1F) << 3; // 5 bits → 8 bits
                // int g = ((color >> 5) & 0x3F) << 2;  // 6 bits → 8 bits
                // int b = (color & 0x1F) << 3;

                // return (uint8_t)(0.299 * r + 0.587 * g + 0.114 * b);
            }

            if (frame->format == PIXFORMAT_GRAYSCALE)
            {
                return (uint8_t)data[(y * width) + x];
            }

            ZS::SerialPort::runtimeException("Only buffer formats available are 'PIXFORMAT_RGB565' and 'PIXFORMAT_GRAYSCALE'");

            return 0; // Avoids compiler warning
        }
    }
}
