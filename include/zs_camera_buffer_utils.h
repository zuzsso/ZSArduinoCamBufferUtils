#ifndef __ZS_CAMERA_BUFFER_UTILS__
#define __ZS_CAMERA_BUFFER_UTILS__
#include <zs_imageometry.h>
#include <esp_camera.h>

namespace ZS
{
    namespace CameraBufferUtils
    {
        void drawLine(ZS::ImaGeometry::ImageLine line, camera_fb_t *frame, uint16_t color);
        void drawCircle(ZS::ImaGeometry::Circle c, camera_fb_t *frame, uint16_t color);
        void drawRectangle(ZS::ImaGeometry::Rectangle r, camera_fb_t *frame, uint16_t color);
        void draw5x3Text(ZS::ImaGeometry::ImageCoordinate c, const char *text, camera_fb_t *frame, uint16_t color);
        void drawConvexArea(ZS::ImaGeometry::ConvexArea area, camera_fb_t *frame, uint16_t color);
        uint8_t getGrayscaleIndex(camera_fb_t *frame, int x, int y);        
    }
}
#endif
