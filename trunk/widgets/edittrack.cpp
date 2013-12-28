#include "winmusik3.h"
#include "edittrack.h"
#include "src/editor/tablesearch.h"
#include "src/editor/shortcutdialog.h"
#include <QClipboard>
#include <QCompleter>
#include <QUrl>
#include <QTimer>
#include <QPixmap>
#include <QBuffer>
#include <QDesktopServices>

EditTrack::EditTrack(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	position=0;
	oldposition=0;
	fkeys=NULL;
	titleCompleter=NULL;
	artistCompleter=NULL;
	albumCompleter=NULL;
	currentTrackListItem=NULL;
	searchWindow=NULL;
	TrackList=NULL;
	DupeCheck=false;
	DupeTimer=NULL;
	DupeCheckIcon=":/fkeys/resources/fkeys/f-key-2005.png";

	ui.releaseDate->setDate(QDate::currentDate());
	ui.recordDate->setDate(QDate::currentDate());

	// Events capturen
	InstallFilter(ui.artist,4);
	InstallFilter(ui.title,5);
	InstallFilter(ui.versionId,6);
	InstallFilter(ui.version,7);
	InstallFilter(ui.genreId,8);
	InstallFilter(ui.genre,9);
	InstallFilter(ui.length,10);
	InstallFilter(ui.bpm,11);
	InstallFilter(ui.musickey,30);
	InstallFilter(ui.bitrate,29);
	InstallFilter(ui.releaseDate,12);
	InstallFilter(ui.recordDate,13);
	InstallFilter(ui.album,14);
	InstallFilter(ui.labelId,15);
	InstallFilter(ui.labelName,16);
	InstallFilter(ui.recordSourceId,17);
	InstallFilter(ui.recordSource,18);
	InstallFilter(ui.recordDeviceId,19);
	InstallFilter(ui.recordDevice,20);
	InstallFilter(ui.remarks,21);
	InstallFilter(ui.tags,22);
	InstallFilter(ui.complete,23);
	InstallFilter(ui.realTitle,24);
	InstallFilter(ui.interrupted,25);
	InstallFilter(ui.channels,26);
	InstallFilter(ui.quality,27);
	InstallFilter(ui.rating,28);


}

EditTrack::~EditTrack()
{

}

void EditTrack::setWinMusikClient(CWmClient *wm)
{
	this->wm=wm;
	TCVersion.Title=tr("Version");
	TCVersion.Init(this,wm,ui.versionId,ui.version,&wm->VersionStore);
	TCVersion.SetNextWidget(ui.genreId);

	TCGenre.Title=tr("Genre");
	TCGenre.Init(this,wm,ui.genreId,ui.genre,&wm->GenreStore);
	TCGenre.SetNextWidget(ui.length);

	TCLabel.Title=tr("Label");
	TCLabel.Init(this,wm,ui.labelId,ui.labelName,&wm->LabelStore);
	TCLabel.SetNextWidget(ui.recordSourceId);

	TCRecordSource.Title=tr("Record Source");
	TCRecordSource.Init(this,wm,ui.recordSourceId,ui.recordSource,&wm->RecordSourceStore);
	TCRecordSource.SetNextWidget(ui.recordDeviceId);

	TCRecordDevice.Title=tr("Record Device");
	TCRecordDevice.Init(this,wm,ui.recordDeviceId,ui.recordDevice,&wm->RecordDeviceStore);
	TCRecordDevice.SetNextWidget(ui.remarks);
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
	ui.version->setText(data.Version);
	if (Ti.VersionId) {
		Tmp.Setf("%u",Ti.VersionId);
		ui.version->setText(wm->GetVersionText(Ti.VersionId));
	} else Tmp="*";
	ui.versionId->setText(Tmp);


	// Genre
	ui.genre->setText(data.Genre);
	if (Ti.GenreId) {
		Tmp.Setf("%u",Ti.GenreId);
		ui.genre->setText(wm->GetGenreText(Ti.GenreId));
	} else Tmp="*";
	ui.genreId->setText(Tmp);


	// Länge
	if (Ti.Length>0) Tmp.Setf("%0i:%02i",(int)(Ti.Length/60),Ti.Length%60); else Tmp.Clear();
	ui.length->setText(Tmp);

	// BPM
	if (Ti.BPM>0) Tmp.Setf("%i",Ti.BPM); else Tmp.Clear();
	ui.bpm->setText(Tmp);

	// Music Key
	ui.musickey->setText(Ti.getKeyName(wm->conf.musicKeyDisplay));

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
	ui.labelName->setText(data.Label);
	if (Ti.LabelId) {
		ui.labelName->setText(wm->GetLabelText(Ti.LabelId));
		Tmp.Setf("%u",Ti.LabelId);
	} else Tmp="";
	ui.labelId->setText(Tmp);

	// Aufnahmequelle
	ui.recordSource->setText(data.RecordingSource);
	if (Ti.RecordSourceId) {
		Tmp.Setf("%u",Ti.RecordSourceId);
		ui.recordSource->setText(wm->GetRecordSourceText(Ti.RecordSourceId));
	} else Tmp="";
	ui.recordSourceId->setText(Tmp);

	// Aufnahmegerät
	ui.recordDevice->setText(data.RecordingDevice);
	if (Ti.RecordDeviceId) {
		Tmp.Setf("%u",Ti.RecordDeviceId);
		ui.recordDevice->setText(wm->GetRecordDeviceText(Ti.RecordDeviceId));
	} else Tmp="";
	ui.recordDeviceId->setText(Tmp);

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

void EditTrack::setFocus()
{
	ui.artist->setFocus();
}

void EditTrack::setFocus (Qt::FocusReason reason)
{
	this->setFocus(reason);
	ui.artist->setFocus(reason);
}

void EditTrack::setFkeys(Fkeys *fkeys)
{
	this->fkeys=fkeys;
}

const TrackInfo &EditTrack::getData()
{
	ppl6::CString Tmp, Artist, Title;
	// Wir müssen sicherstellen, dass die Tabellen-Index-Felder gültige Werte enthalten
	if (!TCVersion.Finish()) {
		ui.versionId->setFocus();
		throw DataIncompleteException();
	}
	if (!TCGenre.Finish()) {
		ui.genreId->setFocus();
		throw DataIncompleteException();
	}
	if (!TCLabel.Finish()) {
		ui.labelId->setFocus();
		throw DataIncompleteException();
	}
	if (!TCRecordSource.Finish()) {
		ui.recordSourceId->setFocus();
		throw DataIncompleteException();
	}
	if (!TCRecordDevice.Finish()) {
		ui.recordDeviceId->setFocus();
		throw DataIncompleteException();
	}


	// Interpret und Titel
	Artist=ui.artist->text();
	Artist.Trim();
	data.Ti.SetArtist(Artist);
	Title=ui.title->text();
	Title.Trim();
	data.Ti.SetTitle(Title);

	if ((Artist.IsEmpty()) && (Title.IsEmpty())) {
		// Warning
		int ret=QMessageBox::warning(this, tr("WinMusik: Attention"),
				tr("Artist and title are empty!\nDo you realy want to save this track?"),
				QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
		if (ret==QMessageBox::No) {
			ui.artist->setFocus();
			throw DataIncompleteException();
		}
	}

	// Version
	data.Ti.VersionId=ui.versionId->text().toInt();
	data.Version=ui.version->text();

	// Genre
	data.Ti.GenreId=ui.genreId->text().toInt();
	data.Genre=ui.genre->text();

	// Länge
	data.Ti.Length=Time2Int(Tmp=ui.length->text());

	// BPM
	data.Ti.BPM=ui.bpm->text().toInt();

	// Music Key
	data.Ti.SetKey(ui.musickey->text());

	// Bitrate
	data.Ti.Bitrate=ui.bitrate->text().toInt();

	// Erscheinungsjahr
	QDate Date=ui.releaseDate->date();
	Tmp=Date.toString("yyyyMMdd");
	data.Ti.ReleaseDate=Tmp.ToInt();

	// Aufnahmedatum
	Date=ui.recordDate->date();
	Tmp=Date.toString("yyyyMMdd");
	//printf ("Date: %s\n",(const char*)Tmp);
	data.Ti.RecordDate=Tmp.ToInt();

	// Album
	Tmp=ui.album->text();
	data.Ti.SetAlbum(Tmp);

	// Label
	data.Ti.LabelId=ui.labelId->text().toInt();
	data.Label=ui.labelName->text();

	// Aufnahmequelle
	data.Ti.RecordSourceId=ui.recordSourceId->text().toInt();
	data.RecordingSource=ui.recordSource->text();

	// Aufnahmegerät
	data.Ti.RecordDeviceId=ui.recordDeviceId->text().toInt();
	data.RecordingDevice=ui.recordDevice->text();

	// Bemerkung
	Tmp=ui.remarks->text();
	data.Ti.SetRemarks(Tmp);

	// Tags
	Tmp=ui.tags->text();
	Tmp=ppl6::Trim(ppl6::LCase(Tmp));
	data.Ti.SetTags(Tmp);

	// Flags
	if (ui.complete->isChecked()) data.Ti.Flags|=1;
	else data.Ti.Flags&=(0xff-1);
	if (ui.realTitle->isChecked()) data.Ti.Flags|=2;
	else data.Ti.Flags&=(0xff-2);
	if (ui.interrupted->isChecked()) data.Ti.Flags|=8;
	else data.Ti.Flags&=(0xff-8);

	// Ton
	data.Ti.Channels=ui.channels->currentIndex();

	// Qualität
	data.Ti.Quality=ui.quality->currentIndex();

	// Bewertung
	data.Ti.Rating=ui.rating->currentIndex();

	// Cover
	if (data.Ti.DeviceType==7) {
		ppl6::CString Path=wm->MP3Filename(data.Ti.DeviceId,data.Ti.Page,data.Ti.Track);
		if (Path.NotEmpty()) {
			ppl6::CDirEntry de;
			if (ppl6::CFile::Stat(Path,de)) {
				data.Ti.Size=de.Size;
				if (Cover.isNull()) {
					data.Ti.CoverPreview.Clear();
				} else {
					QPixmap icon=Cover.scaled(64,64,Qt::KeepAspectRatio,Qt::SmoothTransformation);
					QByteArray bytes;
					QBuffer buffer(&bytes);
					buffer.open(QIODevice::WriteOnly);
					icon.save(&buffer, "JPEG",70);
					data.Ti.CoverPreview.Copy(bytes.data(),bytes.size());
				}
			}
		}
	}
	return data;
}

void EditTrack::ReloadTranslation()
{
	ui.retranslateUi(this);
}

void EditTrack::InstallFilter(QObject *object, int id)
{
	object->installEventFilter(this);
	object->setProperty("id",id);
}

/*!\brief Event Handler
 *
 * Dies ist eine überladene Funktion, die von Qt immer dann aufgerufen wird, wenn bei einem Widget, bei dem ein
 * Filter installiert wurde (Edit::InstallFilter) ein beliebiger Event auftritt. Die Funktion ruft lediglich
 * die Funktion Edit::consumeEvent auf. Liefert diese \c true zurück, wird davon ausgegangen, dass der
 * Event behandelt wurde und der Default-Eventhandler (QWidget::eventFilter) nicht aufgerufen werden muss.
 *
 * \param[in] target Pointer auf das QObject, für den das Event bestimmt ist
 * \param[in] event Pointer auf eine QEvent Klasse, die den Event näher beschreibt
 * \returns Liefert \c true zurück, wenn der Event von Edit::consumeEvent verarbeitet wurde. Andernfalls
 * wird die übergeordnete Basisfunktion QWidget::eventFilter aufgerufen und deren Returncode zurückgeliefert.
 */
bool EditTrack::eventFilter(QObject *target, QEvent *event)
{
	if (consumeEvent(target,event)) return true;
	return QWidget::eventFilter(target,event);
}


/*!\brief Event verarbeiten
 *
 * Diese Funktion wird von Edit::eventFilter aufgerufen, wenn Qt einen Event für ein Widget registriert hat,
 * für das ein Filter installiert wurde (Edit::InstallFilter).
 *
 * Die Funktion prüft, ob für das angegebene \p target ein Handler existiert und ruft diesen auf. Der
 * spezielle Handler muss \c true zurückgeben, wenn er den Event verarbeitet hat und der Standardhandler
 * von Qt nicht mehr aufgerufen werden muss. Die Rückgabe von \c false bewirkt, dass der
 * Standardhandler QWidget::eventFilter aufgerufen wird.
 *
 * \param[in] target Pointer auf das QObject, für den das Event bestimmt ist
 * \param[in] event Pointer auf eine QEvent Klasse, die den Event näher beschreibt
 * \returns Gibt \c true zurück, wenn der Event verarbeit wurde, sonst \c false
 */
bool EditTrack::consumeEvent(QObject *target, QEvent *event)
{
	ppl6::CString Tmp;
	QKeyEvent *keyEvent=NULL;
	int key=0;
	int modifier=Qt::NoModifier;
	QFocusEvent *focusEvent=NULL;

	// Id auslesen
	int id=target->property("id").toInt();
	int type=event->type();
	if (type==QEvent::KeyPress) {
		keyEvent= static_cast<QKeyEvent *>(event);
		key=keyEvent->key();
		modifier=keyEvent->modifiers();
		if (checkKeyPress(target,key,modifier)) return true;		// Fkeys und andere Steuerkeys prüfen
	} else if (type==QEvent::FocusIn || type==QEvent::FocusOut) {
		focusEvent=static_cast<QFocusEvent *>(event);
		if (type==QEvent::FocusIn) {
			position=id;
			UpdateFkeys();
			if (position>7 && DupeCheck==false) {
				CheckDupes();
			} else if (position<8 && DupeCheck==true) DupeCheck=false;
		}
		else if (type==QEvent::FocusOut) {
			oldposition=id;
			if (oldposition==10) on_length_FocusOut();
		}
	}

	if (target==ui.artist) {
		if (type==QEvent::FocusIn) {

		}
	} else if (target==ui.title) {
	} else if (target==ui.version || target==ui.versionId) {
		return TCVersion.ConsumeEvent(target,event,oldposition,position);
	} else if (target==ui.genre || target==ui.genreId) {
		return TCGenre.ConsumeEvent(target,event,oldposition,position);
	} else if (target==ui.labelName || target==ui.labelId) {
		return TCLabel.ConsumeEvent(target,event,oldposition,position);
	} else if (target==ui.recordSource || target==ui.recordSourceId) {
		return TCRecordSource.ConsumeEvent(target,event,oldposition,position);
	} else if (target==ui.recordDevice || target==ui.recordDeviceId) {
		return TCRecordDevice.ConsumeEvent(target,event,oldposition,position);
	} else if (target==ui.album) {
	} else if (target==ui.length) {
	} else if (target==ui.bpm) {
	} else if (target==ui.musickey) {
	} else if (target==ui.bitrate) {
	} else if (target==ui.album) {
	} else if (target==ui.remarks) {
	} else if (target==ui.tags) {
	}
	return false;
}

void EditTrack::UpdateFkeys()
{
	if (fkeys) {
		fkeys->setFkey(0,":/fkeys/resources/fkeys/f-key-0000.png"," ",false);
		fkeys->setFkey(1,":/fkeys/resources/fkeys/f-key-0001.png"," ",false);
		fkeys->setFkey(2,":/fkeys/resources/fkeys/f-key-0002.png"," ",false);
		fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-0003.png"," ",false);
		fkeys->setFkey(4,":/fkeys/resources/fkeys/f-key-0004.png"," ",false);
		fkeys->setFkey(5,":/fkeys/resources/fkeys/f-key-0005.png"," ",false);
		fkeys->setFkey(6,":/fkeys/resources/fkeys/f-key-0006.png"," ",false);
		fkeys->setFkey(7,":/fkeys/resources/fkeys/f-key-0007.png"," ",false);
		fkeys->setFkey(8,":/fkeys/resources/fkeys/f-key-0008.png"," ",false);
		fkeys->setFkey(9,":/fkeys/resources/fkeys/f-key-0009.png"," ",false);
		fkeys->setFkey(10,":/fkeys/resources/fkeys/f-key-0010.png"," ",false);
		fkeys->setFkey(11,":/fkeys/resources/fkeys/f-key-0011.png"," ",false);
		fkeys->setFkey(12,":/fkeys/resources/fkeys/f-key-0012.png"," ",false);

		QString t[20];
		t[0]=tr("close");
		t[1]=tr("new");
		t[2]=tr("back");
		t[3]=tr("master data");
		t[4]=tr("search");
		t[5]=tr("print");
		t[6]=tr("save");
		t[7]=tr("delete");
		t[8]=tr("insert");
		t[9]=tr("first letter");
		t[10]=tr("read ID3");
		t[11]=tr("shortcut");
		t[12]=tr("original Tags");
		t[13]=tr("dupes?");
		t[14]=tr("playlist");
		t[15]=tr("autoimport");
		t[16]=tr("save all ID3");
		t[17]=tr("list devices");
		t[18]=tr("mass import");

		switch (position) {
			case 4:		// Interpret
				fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
				fkeys->setFkey(5,":/fkeys/resources/fkeys/f-key-1005.png",t[11]);
				break;
			case 5:		// Titel
				fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
				break;
			case 6:		// VersionId
				break;
			case 7:		// Version
				fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
				break;
			case 9:		// Genre
				fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
				break;
			case 14:		// Album
				fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
				break;
			case 16:		// Label
				fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
				break;
			case 18:		// Aufnahmequelle
				fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
				break;
			case 20:		// Aufnahmegerät
				fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
				break;
			case 21:		// Bemerkung
				fkeys->setFkey(3,":/fkeys/resources/fkeys/f-key-1003.png",t[9]);
				break;
		}
		if (position>1) {
			fkeys->setFkey(0,":/fkeys/resources/fkeys/f-key-2000.png",t[2]);
		}
		if (position>3) {
			fkeys->setFkey(12,":/fkeys/resources/fkeys/f-key-1012.png",t[6]);
			if (wm->conf.DevicePath[data.Ti.DeviceType].NotEmpty()==true) {
				fkeys->setFkey(6,":/fkeys/resources/fkeys/f-key-1006.png",t[10]);
				if (data.Ti.ImportData>0) fkeys->setFkey(9,":/fkeys/resources/fkeys/f-key-1009.png",t[12]);
			}
		}
		if (position>7) {
			// DupeCheck
			fkeys->setFkey(5,DupeCheckIcon,t[13]);
		}

		// Suchbutton
		if (position==4 || position==5 || position==6 || position==8
				||position==15 || position==17 || position==19) {
			fkeys->setFkey(4,":/fkeys/resources/fkeys/f-key-1004.png",t[4]);
		}
	}
	emit updateFkeys(position);
}


void EditTrack::CheckDupes()
{
	DupeCheckIcon=":/fkeys/resources/fkeys/f-key-2005.png";
	DupeCheck=true;
	TCVersion.Finish();
	ppl6::CString Tmp, Artist, Title;
	// Interpret und Titel
	Artist=ui.artist->text();
	Artist.Trim();
	Title=ui.title->text();
	Title.Trim();
	ppluint32 Version=ui.versionId->text().toInt();
	if (wm->Hashes.CheckDupes(Artist,Title,Version,data.Ti.TitleId)) {
		if (!DupeTimer) {
			DupeTimer=new QTimer(this);
			connect(DupeTimer, SIGNAL(timeout()), this, SLOT(on_DupeTimer_update()));
		}
		DupeTimer->start(300);
	} else if (DupeTimer) DupeTimer->stop();
}

void EditTrack::on_DupeTimer_update()
{
	static int count=0;
	count++;
	if (count&1) DupeCheckIcon=":/fkeys/resources/fkeys/f-key-3005.png";
	else DupeCheckIcon=":/fkeys/resources/fkeys/f-key-2005.png";
	if (position>7) {
		if (fkeys) fkeys->setFkey(5,DupeCheckIcon,"");
	}
}

bool EditTrack::checkKeyPress(QObject *target, int key, int modifier)
/*!\brief Globale KeyPress Events behandeln
 *
 * Diese Funktion behandelt globale KeyPress Events, als Beispielsweise das Drücken der ESC-Taste,
 * F1-F12 aber auch die Return/Enter-Taste und weitere Shortcuts.
 *
 * \param[in] target Das QObject, auf das sich der Event bezieht
 * \param[in] key Code der Taste (Qt::Key)
 * \param[in] modifier Modifizierung dazu (Qt::KeyboardModifiers)
 *
 * \returns Die Funktion gibt \b true zurück, wenn der Event behandelt wurde und das System sonst
 * nichts mehr machen muss, andernfalls \b false.
 */
{
	// ******************************************************************************* ESC
	if (key==Qt::Key_Escape) {
		if (position>3) ui.artist->setFocus();
		emit escPressed();
		return true;
		// *************************************************************************** Return/Enter
	} else if (key==Qt::Key_Return || key==Qt::Key_Enter) {
		MoveToNextWidget();
		return true;
		// *************************************************************************** Page down
	} else if (key==Qt::Key_PageDown) {
		emit pageDownPressed();
		return true;
		// *************************************************************************** Page up
	} else if (key==Qt::Key_PageUp) {
		emit pageUpPressed();
		return true;
		// *************************************************************************** F2
		/*
	} else if (key==Qt::Key_F2 && modifier==Qt::NoModifier && position>1) {
		ppluint32 ret=EditDeviceDialog(DeviceId);
		((QWidget*)target)->setFocus();
		if (ret==DeviceId) UpdateDevice();
		return true;
		// *************************************************************************** F3
	} else if (key==Qt::Key_F3 && modifier==Qt::NoModifier && fkeys->isEnabled(3)==true) {
		QLineEdit *LineEdit=(QLineEdit*)target;
		ppl6::CWString Tmp=LineEdit->text().toLower();
		Tmp.UCWords();
		LineEdit->setText(Tmp);
		return true;
		// *************************************************************************** F4
	} else if (key==Qt::Key_F4 && position>=4 && position<=5) {
		return on_f4_Pressed(position);
		// *************************************************************************** F5
	} else if (key==Qt::Key_F5 && position==4) {
		return on_f5_ShortCut(modifier);
	} else if (key==Qt::Key_F5 && position>7) {
		return on_f5_CheckDupes(target);
		// *************************************************************************** F6
	} else if (DeviceType==7 && key==Qt::Key_F6 && modifier==Qt::ControlModifier && fkeys->isEnabled(6)==true && position>3 && wm->conf.DevicePath[DeviceType].NotEmpty()==true) {
		wm->TrashMP3File(DeviceId,Page,Track.Track);
		return on_f6_Pressed(target,Qt::NoModifier);

	} else if (DeviceType==7 && key==Qt::Key_F6 && fkeys->isEnabled(6)==true && position>3 && wm->conf.DevicePath[DeviceType].NotEmpty()==true) {
		return on_f6_Pressed(target,modifier);

	} else if (DeviceType==7 && key==Qt::Key_F6 && fkeys->isEnabled(6)==true && position==3 && wm->conf.DevicePath[DeviceType].NotEmpty()==true) {
		return on_f6_MassImport();
		// *************************************************************************** F7
	} else if (key==Qt::Key_F7 && position>3 && modifier==Qt::NoModifier) {
		return on_f7_DeleteTrack();
		// *************************************************************************** F8
	} else if (key==Qt::Key_F8 && position>3 && modifier==Qt::NoModifier) {
		return on_f8_InsertTrack();
		*/
		// *************************************************************************** F9
	} else if (data.Ti.DeviceType==7 && key==Qt::Key_F9 && modifier==Qt::NoModifier && position>3 && data.Ti.ImportData>0) {
		DataOimp	Oimp;
		if (wm->OimpDataStore.GetCopy(data.Ti.ImportData,&Oimp)) {
			emit showOimpData(Oimp);
		}
		return true;
		// *************************************************************************** F9
		/*
	} else if (DeviceType==7 && key==Qt::Key_F9 && modifier==Qt::NoModifier && position==3 && wm->conf.DevicePath[DeviceType].NotEmpty()==true) {
		return on_f9_UpdateAllID3Tags();
		// *************************************************************************** F10
	} else if (key==Qt::Key_F10 && position>2 && modifier==Qt::NoModifier && DeviceType==7 && wm->conf.DevicePath[DeviceType].NotEmpty()==true) {
		return on_f10_WritePlaylist();
		// *************************************************************************** F11 => Printing
	} else if (key==Qt::Key_F11 && position>2 && modifier==Qt::NoModifier) {
		wm->PrintCoverDialog(this,DeviceType,DeviceId);
		return true;
		*/
		// *************************************************************************** F12
	} else if (key==Qt::Key_F12 && modifier==Qt::NoModifier) {
		emit save();
		return true;
		// *************************************************************************** Alt & b
	} else if (key==Qt::Key_B && modifier==Qt::AltModifier) {
		ui.remarks->setFocus();
		return true;
		// *************************************************************************** Alt & a
	} else if (key==Qt::Key_A && modifier==Qt::AltModifier) {
		ui.album->setFocus();
		return true;
		// *************************************************************************** Alt & d
	} else if (key==Qt::Key_D && modifier==Qt::AltModifier) {
		ui.recordDate->setFocus();
		return true;
		// *************************************************************************** Alt & l
	} else if (key==Qt::Key_L && modifier==Qt::AltModifier) {
		ui.labelId->setFocus();
		return true;
		// *************************************************************************** Alt & t
	} else if (key==Qt::Key_T && modifier==Qt::AltModifier) {
		ui.tags->setFocus();
		return true;
		// *************************************************************************** Alt & y
	} else if (key==Qt::Key_Y && modifier==Qt::AltModifier) {
		ui.releaseDate->setFocus();
		return true;
		// *************************************************************************** Alt & j
	} else if (key==Qt::Key_J && modifier==Qt::AltModifier) {
		ui.releaseDate->setFocus();
		return true;
	}
	return false;
}

void EditTrack::MoveToNextWidget()
/*!\brief zum nächsten Eingabefeld wechseln
 *
 * Diese Funktion prüft anhand der Variablen \c position in welchem Eingabefeld sich der Focus
 * gerade befindet und setzt den Focus auf das nachfolgende Eingabefeld.
 */
{
	switch (position) {
		case 3: ui.artist->setFocus(); break;
		case 4: ui.title->setFocus(); break;
		case 5: ui.versionId->setFocus(); break;
		case 6: ui.version->setFocus(); break;
		case 7: ui.genreId->setFocus(); break;
		case 8: ui.genre->setFocus(); break;
		case 9: ui.length->setFocus(); break;
		case 10: ui.bpm->setFocus(); break;
		case 11: ui.musickey->setFocus(); break;
		case 30: ui.bitrate->setFocus(); break;
		case 29: ui.releaseDate->setFocus(); break;
		case 12: ui.recordDate->setFocus(); break;
		case 13: ui.album->setFocus(); break;
		case 14: ui.labelId->setFocus(); break;
		case 15: ui.labelName->setFocus(); break;
		case 16: ui.recordSourceId->setFocus(); break;
		case 17: ui.recordSource->setFocus(); break;
		case 18: ui.recordDeviceId->setFocus(); break;
		case 19: ui.recordDevice->setFocus(); break;
		case 20: ui.remarks->setFocus(); break;
		case 21: ui.tags->setFocus(); break;
		case 22: ui.artist->setFocus(); break;
		//case 21: ui.complete->setFocus(); break;
		case 24: ui.realTitle->setFocus(); break;
		case 25: ui.interrupted->setFocus(); break;
		case 26: ui.channels->setFocus(); break;
		case 27: ui.quality->setFocus(); break;
		case 28: ui.rating->setFocus(); break;
		case 31: ui.artist->setFocus(); break;
	};
}


bool EditTrack::on_length_FocusOut()
{
	ppl6::CString Tmp;
	ppluint32 l=Time2Int(Tmp=ui.length->text());
	if (l>0) Tmp.Setf("%0i:%02i",(int)(l/60),l%60); else Tmp.Clear();
	ui.length->setText(Tmp);
	return false;
}

