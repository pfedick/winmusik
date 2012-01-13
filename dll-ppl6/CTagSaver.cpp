#include <windows.h>
#include <shlobj.h>
#include <ppl6.h>
#include <ppl6-sound.h>
#include "winmusik.h"


CTagSaver::CTagSaver()
{
	Log.SetLogfile(ppl6::LOG::DEBUG,"p:/wm20-TagSaver.log");
	Log.SetLogLevel(ppl6::LOG::DEBUG,10);
	
}

CTagSaver::~CTagSaver()
{
	ThreadSignalStop();
	ThreadStop();
}

void CTagSaver::Add(char *filename, ppl6::CpplAssocArray *Tags, bool cleartag)
{
	ppl6::CpplAssocArray a;
	a.Set("tags",Tags);
	a.Set("filename",filename);
	if (cleartag) a.Set("cleartag","true");
	else a.Set("cleartag","false");
	Mutex.Lock();
	//if (Todo.Count()==0) Todo.Clear();
	Todo.Set("[]",&a);
	Log.Printf(ppl6::LOG::DEBUG,2,"CTagSaver","Add",__FILE__,__LINE__,"Adding File %s", filename);
	//Log.PrintArray(ppl6::LOG::DEBUG,1,Tags,"CTagSaver::Add tags");
	//Log.PrintArray(ppl6::LOG::DEBUG,1,&Todo,"CTagSaver::Add Todo");
	if (!ThreadIsRunning()) {
		Log.Printf(ppl6::LOG::DEBUG,1,"CTagSaver","Add",__FILE__,__LINE__,"Starte Thread");
		ThreadStart();
		Log.Printf(ppl6::LOG::DEBUG,1,"CTagSaver","Add",__FILE__,__LINE__,"Starte Thread - ok");
	}
	Mutex.Unlock();
	//Log.Printf(ppl6::LOG::DEBUG,1,"Add End");
}


int CTagSaver::UpdateNow(char *filename, ppl6::CpplAssocArray *Tags, int cleartag)
{
	Log.Printf(ppl6::LOG::DEBUG,1,"CTagSaver","UpdateNow",__FILE__,__LINE__,"UpdateNow: %s",filename);
	Log.PrintArray(ppl6::LOG::DEBUG,1,"CTagSaver","UpdateNow",__FILE__,__LINE__,Tags,"tags:");

	ppl6::CID3Tag Tag;
	ppluint32 e;
	if (!Tag.Load(filename)) {
		e=GetErrorCode();
		Log.Printf(ppl6::LOG::DEBUG,1,"CTagSaver","UpdateNow",__FILE__,__LINE__,"UpdateNow Tag.Load failed: %u",e);
		if (e<402 || e> 405) {
			Log.LogError();
			return 0;
		}
	}
	if (cleartag) Tag.ClearTags();
	Tag.SetArtist(Tags->Get("interpret"));
	Tag.SetTitle(Tags->Get("titel"));
	Tag.SetRemixer(Tags->Get("version"));
	Tag.SetYear(Tags->Get("year"));
	Tag.SetGenre(Tags->Get("genre"));
	Tag.SetComment(Tags->Get("comment"));
	Log.Printf(ppl6::LOG::DEBUG,1,"CTagSaver","UpdateNow",__FILE__,__LINE__,"Saving: %s",filename);
	if (!Tag.Save()) {
		ppl6::PushError();
		Log.LogError();
		Log.Printf(ppl6::LOG::DEBUG,1,"CTagSaver","UpdateNow",__FILE__,__LINE__,"Failed: %s",filename);
		ppl6::PopError();
		return 0;
	}
	Log.Printf(ppl6::LOG::DEBUG,1,"CTagSaver","UpdateNow",__FILE__,__LINE__,"Done: %s",filename);
	return 1;
}

void CTagSaver::ThreadMain(void *param)
{
	Log.Printf(ppl6::LOG::DEBUG,1,"CTagSaver","ThreadMain",__FILE__,__LINE__,"ThreadMain");
	ppl6::CpplAssocArray *Job;
	ppl6::CpplAssocArray MyJob;
	ppl6::CpplString key;
	ppluint64 now, retry;
	while (1) {
		if (ThreadShouldStop()) break;
		Mutex.Lock();
		Todo.Reset();
		while (Job=Todo.GetNextArray()) {
			now=ppl6::GetTime();
			if (!Job) break;
			//Log.PrintArray(ppl6::LOG::DEBUG,1,Job,"Job");
			// Gibt es ein Retry Flag?
			retry=ppl6::atoll(Job->Get("retry"));
			if (!retry) break;	// Nein
			if (retry<now) break;	// Retry-Zeitpunkt ist erreicht
		}
		if (!Job) {
			Mutex.Unlock();
			// Nichts zu tun, wir warten
			//Log.Printf(ppl6::LOG::DEBUG,1,"Idle");
			//Log.PrintArray(ppl6::LOG::DEBUG,1,&Todo,"todo");
			ppl6::MSleep(1000);
			continue;
		} else {
			MyJob.Copy(Job);
			Todo.GetCurrentKey(&key);
			Todo.Delete(&key);
			Mutex.Unlock();
			Log.Printf(ppl6::LOG::DEBUG,1,"CTagSaver","ThreadMain",__FILE__,__LINE__,"Key: >>%s<<",(char*)key);
			Log.PrintArray(ppl6::LOG::DEBUG,1,"CTagSaver","ThreadMain",__FILE__,__LINE__,&MyJob,"MyJob");
			if (!UpdateNow(MyJob.Get("filename"),MyJob.GetArray("tags"),MyJob.IsTrue("cleartag"))) {
				// Update hat nicht funktioniert
				Mutex.Lock();
				MyJob.Setf("retry","%llu",ppl6::GetTime()+60);
				Todo.Set(&key,&MyJob);
				Log.Printf(ppl6::LOG::DEBUG,1,"CTagSaver","ThreadMain",__FILE__,__LINE__,"Retry");
				Mutex.Unlock();
			}
		}
	}
}