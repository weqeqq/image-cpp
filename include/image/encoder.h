#pragma once

#include <image/buffer.h>
#include <image/convertor/depth.h>
#include <image/convertor/color.h>
#include <image/convertor/sequence.h>
#include <fstream>
#include <filesystem>
#include <optional>

#include <image/core/format.h>

namespace Image {
namespace _Encoder {

template <Depth::Tp Dh, Color::Tp Cr>
void EncodePNG(
  const Buffer<Dh, Cr> &input, std::vector<std::uint8_t> &output
) {
  EncodePNG(DepthConvertor(ColorConvertor(input).template Convert<Color::RGB>()).template Convert<Depth::Eight>(), output);
}
template <Depth::Tp Dh, Color::Tp Cr>
void EncodePNG(
  const Buffer<Dh, Cr>          &input, 
  const Buffer<Dh, Color::Grayscale> &alpha, std::vector<std::uint8_t> &output
) {
  EncodePNG(
    DepthConvertor(ColorConvertor(input).template Convert<Color::RGB>()).template Convert<Depth::Eight>(),
    DepthConvertor(alpha).template Convert<Depth::Eight>(),
    output
  );
}
template <>
void EncodePNG(
  const Buffer<Depth::Eight, Color::RGB>  &input, 
  const Buffer<Depth::Eight, Color::Grayscale> &alpha, std::vector<std::uint8_t> &output
);
template <>
void EncodePNG<Depth::Eight, Color::RGB>(
  const Buffer<Depth::Eight, Color::RGB> &input, std::vector<std::uint8_t> &output
);
/*template <Depth::Tp depth, Color::Tp color>*/
/*void EncodeJPEG(*/
/*  const Buffer<depth, color> &input, std::vector<std::uint8_t> &output*/
/*);*/
/*template <>*/
/*void EncodeJPEG <Depth::Eight, Color::RGBA>(*/
/*  const Buffer  <Depth::Eight, Color::RGBA> &input, std::vector<std::uint8_t> &output*/
/*);*/
/*template <>*/
/*void EncodeJPEG <Depth::Eight, Color::RGB>(*/
/*  const Buffer  <Depth::Eight, Color::RGB> &input, std::vector<std::uint8_t> &output*/
/*);*/
/*template <>*/
/*void EncodeJPEG <Depth::Eight, Color::Gray>(*/
/*  const Buffer  <Depth::Eight, Color::Gray> &input, std::vector<std::uint8_t> &output*/
/*);*/

template <typename Br, typename Aa = void>
class EncoderCore {
public:

  EncoderCore(Br, Aa) {}

  EncoderCore(Br) {}

}; // EncoderCore

template <Depth::Tp Dh, Color::Tp Cr>
class EncoderCore<Buffer<Dh, Cr>> {
public:

  EncoderCore(const Buffer<Dh, Cr> &input) : input_(input) {}

  void Encode(std::vector<std::uint8_t> &output, Format::Tp format) const {
    switch (format) {
      case Format::PNG: { 
        EncodePNG(input_, output);
        break;
      }
      default: throw std::runtime_error("UndefinedFormat");
    }
  }

private:
  const Buffer<Dh, Cr> &input_;

}; // EncoderCore<Buffer<Dh, Cr>>

template <Depth::Tp Dh, Color::Tp Cr>
class EncoderCore<Buffer<Dh, Cr>, Buffer<Dh, Color::Grayscale>> {
public:

  EncoderCore(const Buffer<Dh, Cr> &input, const Buffer<Dh, Color::Grayscale> &alpha) 
    : input_(input)
    , alpha_(alpha) {}

  void Encode(std::vector<std::uint8_t> &output, Format::Tp format) const {
    switch (format) {
      case Format::PNG: { 
        EncodePNG(input_, alpha_, output);
        break;
      }
      default: throw std::runtime_error("UndefinedFormat");
    }
  }

private:
  const Buffer<Dh, Cr>          &input_;
  const Buffer<Dh, Color::Grayscale> &alpha_;

}; // EncoderCore<Buffer<Dh, Cr>, Buffer<Dh, Color::Gray>>

};


template <typename... Ar>
class Encoder : public _Encoder::EncoderCore<Ar...> {
private:

  class PNG;
  class JPEG;
  
public:

  Encoder(const Ar&... argument_list) : _Encoder::EncoderCore<Ar...>(argument_list...) {}

  void Write(std::ofstream &ofstream, Format::Tp format) const {
    std::vector<std::uint8_t> buffer;
    _Encoder::EncoderCore<Ar...>::Encode(buffer, format);

    ofstream.write(reinterpret_cast<char *>(buffer.data()), buffer.size());
  }
  void Write(const char *path, Format::Tp format = Format::Undefined) const {
    std::ofstream ofstream(path, std::ios::binary);

    if (!ofstream) {
      throw std::runtime_error("Failed to open output file stream");
    }
    Write(ofstream, format == Format::Undefined ? Format::FromPath(path) : format);
    ofstream.close();
  }
  void Write(const std::string &path, Format::Tp format = Format::Undefined) const {
    Write(path.c_str(), format);
  }

}; // Encoder

template <Depth::Tp depth, Color::Tp color>
void Write(
  const Buffer<depth, color> &input, std::vector<std::uint8_t> &output, Format::Tp format
) {
  Encoder(input).Write(output, format);
}
template <Depth::Tp depth, Color::Tp color>
void Write(
  const Buffer<depth, color> &input, std::ofstream &output, Format::Tp format
) {
  Encoder(input).Write(output, format);
}
template <Depth::Tp depth, Color::Tp color>
void Write(
  const Buffer<depth, color> &input, const char *output, Format::Tp format = Format::Undefined
) {
  Encoder(input).Write(output, format);
}
template <Depth::Tp depth, Color::Tp color>
void Write(
  const Buffer<depth, color> &input, const std::string &output, Format::Tp format = Format::Undefined
) {
  Encoder(input).Write(output, format);
}

}; // Image
