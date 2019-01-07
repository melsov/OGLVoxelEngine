#ifndef SHADER_HPP
#define SHADER_HPP

bool LoadShaders(const char * vertex_file_path, const char * fragment_file_path, GLuint& programID);
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

#endif
