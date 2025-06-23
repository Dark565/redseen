/*
 *  Copyright (C) 2025 Grzegorz Kociołek (grzegorz.kclk@gmail.com)
 *
 *  This file is a part of RedSeen; a 3D game engine.
 *
 *  RedSeen is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RedSeen is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <string>

namespace redseen::render {

class Shader {
  public:
    // Constructor for loading from files
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    // Constructor for direct source code
    Shader(const char *vertexSource, const char *fragmentSource,
           bool isSourceCode);

    void use() const;
    unsigned int ID;

  private:
    std::string readFile(const std::string &filePath) const;
    void checkCompileErrors(unsigned int shader, const std::string &type) const;
    void compileAndLink(const char *vertexCode, const char *fragmentCode);
};

} // namespace redseen::render