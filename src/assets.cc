#pragma once

#include "assets.h"

std::string ASSETS::ImagePath(char * image)
{
#ifdef _WIN32
  std::string return_path_ = "../../../../assets/images/";
  return_path_ = return_path_ + image + "\0";
  return return_path_;
#elif __APPLE__
  std::string return_path_ = "assets/images/";
  return_path_ = return_path_ + image + "\0";
  return return_path_;
#endif
}

std::string ASSETS::SoundPath(char * sound)
{
#ifdef _WIN32
  std::string return_path_ = "../../../../assets/sound/";
  return_path_ = return_path_ + sound;
  return return_path_;
#elif __APPLE__
  std::string return_path_ = "assets/sound/";
  return_path_ = return_path_ + sound + "\0";
  return return_path_;
#endif
}
