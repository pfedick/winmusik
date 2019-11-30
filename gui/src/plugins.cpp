#include "plugins.h"
#ifdef HAVE_PYTHON
#include <Python.h>
#endif

Plugins::Plugins()
{

}


Plugins::~Plugins()
{

}


void Plugins::loadFromPath(const ppl6::CString &Path)
{
#ifdef HAVE_PYTHON
	ppl6::CDir Dir;
	if (!Dir.Open(Path)) return;

	PyObject *sysPath = PySys_GetObject("path");
	PyObject *path = PyString_FromString(Path);
	int result = PyList_Insert(sysPath, 0, path);
	Py_DECREF(path);


	Dir.Reset();
	const ppl6::CDirEntry *entry;
	while ((entry=Dir.GetNextPattern("*.py"))) {
		ppl6::CArray matches;
		if (entry->Filename.PregMatch("/^(.*?)\\.py$/i",matches)) {
			ppl6::CString ModuleName=matches.GetString(1);
			printf ("Loading Plugin: %s\n",(const char*)ModuleName);
			python_modules.push_back(ModuleName);
			PyRun_SimpleString("import "+ModuleName+"\n");
		}
	}
#endif
}
