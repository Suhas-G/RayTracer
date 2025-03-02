#include <core/assert.h>
#include <core/image.h>

#ifdef GDIPLUS_SUPPORT
#pragma warning(push, 0)
#include <windows.h>
#include <process.h>
#include <gdiplus.h>
#pragma warning(pop)
#else
#include <png.h>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#endif

#define MINIZ_NO_ARCHIVE_APIS 1
#ifndef __APPLE__
#define TINYEXR_USE_OPENMP 1
#endif
#define TINYEXR_IMPLEMENTATION

// undefine harmful macros set by windef
#undef min
#undef max

#include <tinyexr.h>

namespace rt {

Image::Image( uint width, uint height ) : width_(width), height_(height) {
    pixels = new RGBColor[width*height];
}

Image::Image() : pixels(nullptr), width_(0), height_(0) {
}

Image::Image( const Image& other ) :  pixels(nullptr), width_(0), height_(0) {
    *this = other;
}

void Image::destroy() {
    if (pixels)
        delete [] pixels;
    width_ = 0;
    height_ = 0;
    pixels = nullptr;
}

void Image::create( uint width, uint height ) {
    destroy();
    width_ = width;
    height_ = height;
    pixels = new RGBColor[width*height];
}


Image::~Image() {
    destroy();
}

RGBColor& Image::operator()( uint x, uint y ) {
    rt_assert(x < width_ && y < height_) << "Coordinates [" << x << "," << y << "] are out of bounds of an image [" << width_ << "," << height_ << "]";
    return pixels[y*width_+x];
}

const RGBColor& Image::operator()( uint x, uint y ) const {
    rt_assert(x < width_ && y < height_) << "Coordinates [" << x << "," << y << "] are out of bounds of an image [" << width_ << "," << height_ << "]";
    return pixels[y*width_+x];
}

void Image::operator()(uint x, uint y, const RGBColor& c ) const {
    rt_assert(x < width_ && y < height_) << "Coordinates [" << x << "," << y << "] are out of bounds of an image [" << width_ << "," << height_ << "]";
    pixels[y*width_+x].r = c.r;
    pixels[y*width_+x].g = c.g;
    pixels[y*width_+x].b = c.b;
}

void Image::clear( const RGBColor& color ) {
    for (uint i=0; i<width_*height_; ++i)
        pixels[i] = color;
}

#ifdef GDIPLUS_SUPPORT
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    using namespace Gdiplus;

    UINT  num = 0;          // number of Image encoders
    UINT  size = 0;         // size of the Image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if(size == 0)
        return -1;  // Failure

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if(pImageCodecInfo == NULL)
        return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);

    for(UINT j = 0; j < num; ++j)
    {
        if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }    
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

#undef min
#undef max

#endif


#ifndef GDIPLUS_SUPPORT
void abort_(const char * s, ...)
{
    va_list args;
    va_start(args, s);
    vfprintf(stderr, s, args);
    fprintf(stderr, "\n");
    va_end(args);
    abort();
}
#endif

void Image::writePNG(const std::string& _fileName)
{

#ifdef GDIPLUS_SUPPORT
    using namespace Gdiplus;

    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    CLSID encoderClsid;

    Bitmap *Image = new Bitmap(width_, height_, PixelFormat24bppRGB);
    BitmapData data;
    Rect r(0, 0, width_, height_);
    Image->LockBits(&r, ImageLockModeWrite, PixelFormat24bppRGB, &data);

    RGBColor* it = pixels;
    for(uint y = 0; y < height_; y++)
    {
        byte* ptr = (byte*)data.Scan0 + data.Stride * y;
        for(uint x = 0; x < width_; x++)
        {
            RGBColor v = (*it++).clamp();
            *ptr++ = (byte)(v.b * 255);
            *ptr++ = (byte)(v.g * 255);
            *ptr++ = (byte)(v.r * 255);
        }
    }

    Image->UnlockBits(&data);

    // Get the CLSID of the PNG encoder.
    GetEncoderClsid(L"image/png", &encoderClsid);

    std::wstring name(_fileName.begin(), _fileName.end());
    Image->Save(name.c_str(), &encoderClsid, NULL);

    delete Image;
    GdiplusShutdown(gdiplusToken);
#else

    std::vector<png_byte> byteData (width_*height_ * 3);
    std::vector<png_byte>::iterator ptr = byteData.begin();
    for(size_t i = 0; i<width_*height_; ++i)
    {
        RGBColor v = pixels[i].clamp();
        *ptr++ = (unsigned char)(v.r * 255);
        *ptr++ = (unsigned char)(v.g * 255);
        *ptr++ = (unsigned char)(v.b * 255);
    }

    std::vector<png_byte*> rowData(height_);
    for(uint i = 0; i < height_; i++)
        rowData[i] = i * width_ * 3 + &byteData.front();

    /* create file */
    FILE *fp = fopen(_fileName.c_str(), "wb");
    if (!fp)
        abort_("[write_png_file] File %s could not be opened for writing", _fileName.c_str());


    /* initialize stuff */
    png_structp png_ptr;
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
        abort_("[write_png_file] png_create_write_struct failed");

    png_infop info_ptr;
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        abort_("[write_png_file] png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during init_io");


    png_init_io(png_ptr, fp);

    /* write header */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during writing header");

    png_set_IHDR(png_ptr, info_ptr, width_, height_,
        8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    /* write bytes */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during writing bytes");

    png_write_image(png_ptr, (png_byte**)&rowData.front());


    /* end write */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during end of write");

    png_write_end(png_ptr, NULL);

    fclose(fp);
#endif
}

void Image::readPNG(const std::string& filename)
{
#ifdef GDIPLUS_SUPPORT
	using namespace Gdiplus;

	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	std::wstring name(filename.begin(), filename.end());
	Bitmap *bmp = new Bitmap(name.c_str(), FALSE);
    create(bmp->GetWidth(), bmp->GetHeight());

	BitmapData data;
	Rect r(0, 0, width_, height_);
	bmp->LockBits(&r, ImageLockModeWrite, PixelFormat24bppRGB, &data);

	RGBColor* it = pixels;
	for(uint y = 0; y < height_; y++)
	{
		byte* ptr = (byte*)data.Scan0 + data.Stride * y;
		for(uint x = 0; x < width_; x++)
		{
			RGBColor &v = *it++;
			v.b = (float)(*ptr++) / 255.f;
			v.g = (float)(*ptr++) / 255.f;
			v.r = (float)(*ptr++) / 255.f;
		}
	}

	bmp->UnlockBits(&data);
	delete bmp;

	GdiplusShutdown(gdiplusToken);
#else
	png_byte header[8];	// 8 is the maximum size that can be checked

	/* open file and test for it being a png */
	FILE *fp = fopen(filename.c_str(), "rb");
	if (!fp)
		abort_("[read_png_file] File %s could not be opened for reading", filename.c_str());
	size_t rd_size = fread(header, 1, 8, fp);
	if (rd_size != 8 || png_sig_cmp(header, 0, 8))
		abort_("[read_png_file] File %s is not recognized as a PNG file", filename.c_str());


	/* initialize stuff */
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
		abort_("[read_png_file] png_create_read_struct failed");

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		abort_("[read_png_file] png_create_info_struct failed");

	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[read_png_file] Error during init_io");

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

  create(
      png_get_image_width(png_ptr, info_ptr),
      png_get_image_height(png_ptr, info_ptr)
      );

	// int number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

    png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	std::vector<png_byte> byteData (rowbytes * height_);
	std::vector<png_byte*> rowData(height_);
	for(uint i = 0; i < height_; i++)
		rowData[i] = i * rowbytes + &byteData.front();

	/* read file */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[read_png_file] Error during read_image");

	png_read_image(png_ptr, &rowData.front());

	fclose(fp);

	RGBColor* it = pixels;
	for(size_t y = 0; y < height_; y++)
	{
		png_byte *b = rowData[y];
		for(size_t x = 0; x < width_; x++)
		{
			RGBColor &v = *it++;
			v.r = (float)(*b++) / 255.f;
			v.g = (float)(*b++) / 255.f;
			v.b = (float)(*b++) / 255.f;
			if(png_get_channels(png_ptr, info_ptr) == 4)
				b++;
		}
	}
#endif

}

void Image::writeEXR(const std::string& fileName)
{
    EXRHeader header;
    InitEXRHeader(&header);
    EXRImage image;
    InitEXRImage(&image);

    std::vector<float> r_channel(width_ * height_);
    std::vector<float> g_channel(width_ * height_);
    std::vector<float> b_channel(width_ * height_);

    const float* ptrs[] = { b_channel.data(), g_channel.data(), r_channel.data()};

    image.num_channels = 3;
    image.images       = (unsigned char**)ptrs;
    image.width        = width_;
    image.height       = height_;

    // Copy each channel to his own data
    for(size_t i = 0; i < width_*height_; ++i) {
        RGBColor v = pixels[i];
        r_channel[i] = v.r;
        g_channel[i] = v.g;
        b_channel[i] = v.b;
    }

    header.num_channels = image.num_channels;
    header.channels     = new EXRChannelInfo[header.num_channels];

    constexpr size_t BUFFER_MAX = 255;
#ifdef _MSC_VER
    strncpy_s(header.channels[2].name, "R", BUFFER_MAX);
    strncpy_s(header.channels[1].name, "G", BUFFER_MAX);
    strncpy_s(header.channels[0].name, "B", BUFFER_MAX);
#else
    strncpy(header.channels[2].name, "R", BUFFER_MAX);
    strncpy(header.channels[1].name, "G", BUFFER_MAX);
    strncpy(header.channels[0].name, "B", BUFFER_MAX);
#endif

    header.pixel_types           = new int[header.num_channels];
    header.requested_pixel_types = new int[header.num_channels];
    for (int i = 0; i < header.num_channels; ++i) {
        header.pixel_types[i]           = TINYEXR_PIXELTYPE_FLOAT; // pixel type of input image
        header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT; // pixel type of output image to be stored in .EXR
    }

    const char* err = nullptr;
    int ret         = SaveEXRImageToFile(&image, &header, fileName.c_str(), &err);

    delete[] header.channels;
    delete[] header.pixel_types;
    delete[] header.requested_pixel_types;

    if (ret != TINYEXR_SUCCESS) {
		fprintf(stderr, "[write_exr_file] %s", err);
        FreeEXRErrorMessage(err); // free's buffer for an error message
    }
}

Image& Image::operator=( const Image& other ) {
    if (other.pixels) {
        create(other.width(), other.height());
        memcpy(pixels, other.pixels, width_*height_*sizeof(RGBColor));
    } else
        destroy();
    return *this;
}

}

