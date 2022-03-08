#include "EditorUtil.h"
#include <io.h>

std::vector<std::string> CEditorUtil::GetFilesInDirectory(const std::string& path, const std::string& filter)
{
   std::string searching =  path + filter;

	std::vector<std::string> return_;

	_finddata_t fd;
	intptr_t handle = _findfirst(searching.c_str(), &fd);  //현재 폴더 내 모든 파일을 찾는다.

	if (handle == -1)    return return_;

	int result = 0;
	do
	{
		return_.push_back(fd.name);
		result = _findnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);

	return return_;
}
