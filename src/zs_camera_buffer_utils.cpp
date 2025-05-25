#include <zs_camera_buffer_utils.h>
#include <esp_camera.h>
#include <zs_serial_port_utils.h>
#include <zs_imageometry.h>
#include <stdio.h>

namespace ZS
{
    namespace CameraBufferUtils
    {
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
                uint8_t high = data[index + 1];
                uint8_t low = data[index];
                uint16_t color = (high << 8) | low;
                int r = ((color >> 11) & 0x1F) << 3; // 5 bits → 8 bits
                int g = ((color >> 5) & 0x3F) << 2;  // 6 bits → 8 bits
                int b = (color & 0x1F) << 3;

                return (uint8_t)(0.299 * r + 0.587 * g + 0.114 * b);
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
