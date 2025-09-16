/* 
 * This is a test document testing the glsl syntax highlighting plugin and trying to
 * understand the basic structure of shaders in OpenGL (and apparently Vulkan also)
 */

#version 330 core
in type in_variable_name;
in type in_variable_name;

out type out_variable_name;

uniform type uniform_name;

void main() {
	// process input(s) and do some weird graphics stuff
	// ...
	// output processed stuff to output variable
	out_variable_name = weird_stuff_we_processed;
}
