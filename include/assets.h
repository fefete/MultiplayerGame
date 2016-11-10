#pragma once

#ifndef __ASSETS_H__
#define __ASSETS_H__

#include <string>

namespace ASSETS {
  //@param image: The path from the assets/images folder
  //@return: the correct path (it will be different on Windows or IOS)
  std::string ImagePath(char *image);

  //@param image: The path from the assets/images folder
  //@return: the correct path (it will be different on Windows or IOS)
  std::string SoundPath(char *sound);
}

#endif //__ASSETS_H__
