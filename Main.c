#include "Main.h"

inline int to_upper(const int _Chr)
{
	if (_Chr < 0x61 || _Chr > 0x7a) return _Chr;
	return _Chr - 0x20;
}

// Note: since is not required, this method does not
// check if the first char is a digit
int suitable_for_macro(const char* const _Str)
{
	size_t len, i;
	char _Chr;

	len = strlen(_Str);
	for (i = 0; i < len; i++)
	{
		_Chr = _Str[i];
		if ((_Chr < 0x30 || _Chr > 0x39) && (_Chr < 0x41 || _Chr > 0x5a) && _Chr != 0x5f &&
			(_Chr < 0x61 || _Chr > 0x7a)) return 0;
	}

	return 1;
}

int main(int argc, char* argv[])
{
	int argument_i; size_t str_i, str_i2;

	size_t folder_str_len;
	size_t argument_len, argument_maxlen = 0;
	size_t path_buf_size, macro_buf_size;

	char *path_buf, *macro_buf;

	FILE* iterator;

	if (argc <= 2)
	{
		puts("No argument specified"); return 1;
	}

	// Calculating stuff
	folder_str_len = strlen(argv[1]);
	for (argument_i = 2; argument_i < argc; argument_i++)
	{
		argument_len = strlen(argv[argument_i]);
		if (argument_maxlen < argument_len) argument_maxlen = argument_len;
	}
	path_buf_size = folder_str_len + argument_maxlen + 2 + 1;
	macro_buf_size = 2 + argument_maxlen + 2 + 1;

	// Allocating memory
	path_buf = (char*)malloc(path_buf_size * sizeof(char));
	if (path_buf == NULL) return 2;
	for (str_i = 0; str_i < folder_str_len; str_i++) path_buf[str_i] = argv[1][str_i];

	macro_buf = (char*)malloc(macro_buf_size * sizeof(char));
	if (macro_buf == NULL) return 3;
	macro_buf[0] = '_'; macro_buf[1] = '_';

	for (argument_i = 2; argument_i < argc; argument_i++)
	{
		if (suitable_for_macro(argv[argument_i]) == 0)
		{
			puts("Not suitable for file or macro name"); continue;
		}

		argument_len = strlen(argv[argument_i]);

		// Creating path and macro
		str_i = folder_str_len;
		for (str_i2 = 0; str_i2 < argument_len; str_i2++) path_buf[str_i++] = argv[argument_i][str_i2];
		path_buf[str_i++] = '.'; path_buf[str_i++] = 'h'; path_buf[str_i] = 0x00;

		str_i = 2;
		for (str_i2 = 0; str_i2 < argument_len; str_i2++) macro_buf[str_i++] = to_upper(argv[argument_i][str_i2]);
		macro_buf[str_i++] = '_'; macro_buf[str_i++] = 'H'; macro_buf[str_i] = 0x00;

		// Writing header file
		fopen_s(&iterator, path_buf, "w");
		if (iterator == NULL) return 4;
		fprintf(iterator, "#ifndef %s\n#define %s\n\n#endif", macro_buf, macro_buf);
		fclose(iterator);
	}

	// Releasing resources
	free(macro_buf);
	free(path_buf);

	return 0;
}