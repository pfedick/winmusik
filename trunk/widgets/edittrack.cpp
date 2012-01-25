#include "edittrack.h"

EditTrack::EditTrack(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
}

EditTrack::~EditTrack()
{

}

void EditTrack::setWinMusikClient(CWmClient *wm)
{
	this->wm=wm;
}

void EditTrack::setFilename(const ppl6::CString &Filename)
{
	Cover=QPixmap();
	ppl6::CString Tmp;
	if (Filename.IsEmpty()) {
		ui.filename->setText("");
		ui.filesize->setText("");
	} else {
		ui.filename->setText(Filename);
		ppl6::CDirEntry de;
		if (ppl6::CFile::Stat(Filename,de)) {
			Tmp.Setf("%0.1f",(double)de.Size/1048576.0);
			ui.filesize->setText(Tmp);
			ppl6::CID3Tag Tag;
			if (Tag.Load(Filename)) {
				// Cover?
				ppl6::CBinary cover;
				if (Tag.GetPicture(3,cover)) {
					Cover.loadFromData((const uchar*)cover.GetPtr(),cover.GetSize());
					ui.cover->setPixmap(Cover.scaled(128,128,Qt::KeepAspectRatio,Qt::SmoothTransformation));
				}
			}
		}
	}
}

void EditTrack::setData(const TrackInfo &data)
{
	this->data=data;
	ppl6::CString Tmp;
	DataTitle	Ti=data.Ti;

	// TitleId und Version
	Tmp.Setf("%u",Ti.TitleId);
	ui.titleId->setText(Tmp);
	Tmp.Setf("%u",Ti.GetVersion());
	ui.recordVersion->setText(Tmp);

	// Interpret und Titel
	if (Ti.Artist) ui.artist->setText(Ti.Artist);
	else ui.artist->setText("");
	if (Ti.Title) ui.title->setText(Ti.Title);
	else ui.title->setText("");

	// Version
	if (Ti.VersionId) Tmp.Setf("%u",Ti.VersionId); else Tmp="";
	ui.versionId->setText(Tmp);
	ui.version->setText(wm->GetVersionText(Ti.VersionId));

	// Genre
	if (Ti.GenreId) Tmp.Setf("%u",Ti.GenreId); else Tmp="";
	ui.genreId->setText(Tmp);
	ui.genre->setText(wm->GetGenreText(Ti.GenreId));

	// Länge
	if (Ti.Length>0) Tmp.Setf("%0i:%02i",(int)(Ti.Length/60),Ti.Length%60); else Tmp.Clear();
	ui.length->setText(Tmp);

	// BPM
	if (Ti.BPM>0) Tmp.Setf("%i",Ti.BPM); else Tmp.Clear();
	ui.bpm->setText(Tmp);

	// Bitrate
	if (Ti.Bitrate>0) Tmp.Setf("%i",Ti.Bitrate); else Tmp.Clear();
	ui.bitrate->setText(Tmp);

	// Erscheinungsjahr
	QDate Date;
	Tmp.Setf("%u",Ti.ReleaseDate);
	int year=Tmp.Mid(0,4).ToInt();
	int month=Tmp.Mid(4,2).ToInt();
	int day=Tmp.Mid(6,2).ToInt();
	if (!month) month=1;
	if (!day) day=1;
	Date.setDate(year,month,day);
	ui.releaseDate->setDate(Date);

	// Aufnahmedatum
	Tmp.Setf("%u",Ti.RecordDate);
	Date=QDate::fromString(Tmp,"yyyyMMdd");
	ui.recordDate->setDate(Date);


	// Album
	if (Ti.Album) ui.album->setText(Ti.Album);
	else ui.album->setText("");

	// Label
	if (Ti.LabelId) Tmp.Setf("%u",Ti.LabelId); else Tmp="";
	ui.labelId->setText(Tmp);
	ui.labelName->setText(wm->GetLabelText(Ti.LabelId));

	// Aufnahmequelle
	if (Ti.RecordSourceId) Tmp.Setf("%u",Ti.RecordSourceId); else Tmp="";
	ui.recordSourceId->setText(Tmp);
	ui.recordSource->setText(wm->GetRecordSourceText(Ti.RecordSourceId));

	// Aufnahmegerät
	if (Ti.RecordDeviceId) Tmp.Setf("%u",Ti.RecordDeviceId); else Tmp="";
	ui.recordDeviceId->setText(Tmp);
	ui.recordDevice->setText(wm->GetRecordDeviceText(Ti.RecordDeviceId));

	// Bemerkung
	if (Ti.Remarks) ui.remarks->setText(Ti.Remarks);
	else ui.remarks->setText("");

	// Tags
	if (Ti.Tags) ui.tags->setText(Ti.Tags);
	else ui.tags->setText("");


	// Flags
	if (Ti.Flags&1) ui.complete->setChecked(true);
	else ui.complete->setChecked(false);

	if (Ti.Flags&2) ui.realTitle->setChecked(true);
	else ui.realTitle->setChecked(false);

	if (Ti.Flags&8) ui.interrupted->setChecked(true);
	else ui.interrupted->setChecked(false);

	// Ton
	ui.channels->setCurrentIndex(Ti.Channels);

	// Qualität
	ui.quality->setCurrentIndex(Ti.Quality);

	// Bewertung
	ui.rating->setCurrentIndex(Ti.Rating);


}

const TrackInfo &EditTrack::getData() const
{
	return data;
}
