#pragma once

#include <image/buffer.h>
#include <image/convertor/sequence.h>
#include <image/convertor/depth.h>
#include <image/convertor/color.h>
#include <fstream>
#include <filesystem>
#include <image/core/format.h>

namespace Image {

namespace _Decoder {

template <Depth::Tp Dh, Color::Tp Cr>
Buffer<Dh, Cr> DecodePNG(
  const std::vector<std::uint8_t> &input
) {
  Buffer<Depth::Eight, Cr> output;
  if constexpr (Cr == Color::RGB) {
    output = DecodePNG<Depth::Eight, Color::RGB>(input);
  } else {
    output = ColorConvertor(DecodePNG<Depth::Eight, Color::RGB>(input)).template Convert<Cr>();
  }
  if constexpr (Dh == Depth::Eight) {
    return std::move(output);
  } else {
    return DepthConvertor(output).template Convert<Dh>();
  }
}
template <Depth::Tp Dh, Color::Tp Cr>
Buffer<Dh, Cr> 
DecodePNG(
  const std::vector<std::uint8_t> &input, Buffer<Dh, Color::Grayscale> &alpha
) {
  Buffer<Depth::Eight, Cr>          output8;
  Buffer<Depth::Eight, Color::Grayscale> alpha8;

  if constexpr (Cr == Color::RGB) {
    output8 = DecodePNG<Depth::Eight, Color::RGB>(input, alpha8);
  } else {
    output8 = ColorConvertor(DecodePNG<Depth::Eight, Color::RGB>(input, alpha8)).template Convert<Cr>();
  }
  if constexpr (Dh == Depth::Eight) {
    alpha = std::move(alpha8);
    return  std::move(output8);
  } else {
    alpha = DepthConvertor(alpha8)  .template Convert<Dh>();
    return  DepthConvertor(output8) .template Convert<Dh>();
  }
}

template <>
Buffer<Depth::Eight, Color::RGB> 
DecodePNG(
  const std::vector<std::uint8_t> &input, Buffer<Depth::Eight, Color::Grayscale> &alpha
);
template <>
Buffer    <Depth::Eight, Color::RGB> 
DecodePNG <Depth::Eight, Color::RGB>(
  const std::vector<std::uint8_t> &input
);

template <Depth::Tp depth, Color::Tp color>
Buffer<depth, color> DecodeJPEG(
  const std::vector<std::uint8_t> &input
);
template <Depth::Tp Dh>
Buffer<Depth::Eight, Color::RGB> 
DecodeJPEG(
  const std::vector<std::uint8_t> &input, Buffer<Dh, Color::Grayscale> &alpha
);
template <>
Buffer     <Depth::Eight, Color::RGB> 
DecodeJPEG <Depth::Eight, Color::RGB>(
  const std::vector<std::uint8_t> &input
);
template <>
Buffer     <Depth::Eight, Color::Grayscale> 
DecodeJPEG <Depth::Eight, Color::Grayscale>(
  const std::vector<std::uint8_t> &input
);

}; // _Decoder

class Decoder {
private:
  class PNG;
  class JPEG;

public:

  Decoder(const std::vector<std::uint8_t> &data, Format::Tp format) 
    : input_(data) 
    , format_(format) {}

  Decoder(std::ifstream &ifstream, Format::Tp format)
    : Decoder(FromStream(ifstream), format) {}

  Decoder(const char *file_name, Format::Tp format = Format::Undefined) 
    : Decoder(FromFile(file_name), format == Format::Undefined ? Format::FromPath(file_name) : format) {}

  Decoder(const std::string &file_name, Format::Tp format = Format::Undefined) 
    : Decoder(file_name.c_str(), format) {}

  template <Depth::Tp Dh = Depth::Eight, Color::Tp Cr = Color::RGB>
  Buffer<Dh, Cr> Decode() const {
    switch (format_) {
      case Format::PNG  : return _Decoder::DecodePNG  <Dh, Cr>(input_);
      /*case Format::JPEG : return _Decoder::DecodeJPEG <Dh, Cr>();*/
      default: throw std::runtime_error("Undefined Format");
    }
  }
  template <Depth::Tp Dh = Depth::Eight, Color::Tp Cr = Color::RGB>
  Buffer<Dh, Cr> Decode(Buffer<Dh, Color::Grayscale> &alpha) const {
    switch (format_) {
      case Format::PNG  : return _Decoder::DecodePNG  <Dh, Cr>(input_, alpha);
      /*case Format::JPEG : return _Decoder::DecodeJPEG <Dh, Cr>(alpha);*/
      default: throw std::runtime_error("Undefined Format");
    }
  }

private:
  std::vector<std::uint8_t> input_;
  Format::Tp                format_;

  static std::vector<std::uint8_t> FromStream(std::ifstream &ifstream) {
    ifstream.seekg(0, std::ios::end);
    std::streamsize length =  ifstream.tellg();
    ifstream.seekg(0, std::ios::beg);

    std::vector<std::uint8_t> buffer(length);

    if (!ifstream.read(reinterpret_cast<char *>(buffer.data()), length)) {
      throw std::runtime_error("failed to read stream");
    }
    ifstream.close();
    return buffer;
  }

  static std::vector<std::uint8_t> FromFile(const char *file_name) {
    std::ifstream ifstream(file_name, std::ios::binary);
    if (!ifstream) {
      throw std::runtime_error("failed to open stream");
    }
    return FromStream(ifstream);
  }
};

}; // Image
