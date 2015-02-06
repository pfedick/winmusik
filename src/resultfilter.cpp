/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.15 $
 * $Date: 2010/12/23 18:04:40 $
 * $Id: search.cpp,v 1.15 2010/12/23 18:04:40 pafe Exp $
 *
 *
 * Copyright (c) 2014 Patrick Fedick
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "winmusik3.h"
#include "wmstorage.h"
#include "resultfilter.h"


ResultFilter::ResultFilter()
{
	disableAll();
}

void ResultFilter::disableAll()
{
	bpmEnabled=false;
	yearEnabled=false;
	recordingDateEnabled=false;
	musicKeyEnabled=false;
	genresEnabled=false;
	ratingEnabled=false;
	lengthEnabled=false;
	energyEnabled=false;
	bpmStart=0;
	bpmEnd=999;
	yearStart=0;
	yearEnd=9999;
	recordingStart=0;
	recordingEnd=99999999;
	musicKey=0;
	ratingStart=0;
	ratingEnd=6;
	lengthStart=0;
	lengthEnd=9999;
	energyStart=0;
	energyEnd=10;
}

void ResultFilter::setBpmRange(bool enabled, int start, int end)
{
	bpmEnabled=enabled;
	bpmStart=start;
	bpmEnd=end;
}

void ResultFilter::setYearRange(bool enabled, int start, int end)
{
	yearEnabled=enabled;
	yearStart=start*10000;
	yearEnd=end*10000+1231;
}

void ResultFilter::setRatingRange(bool enabled, int start, int end)
{
	ratingEnabled=enabled;
	ratingStart=start;
	ratingEnd=end;
}

void ResultFilter::setRecordingRange(bool enabled, int start, int end)
{
	recordingDateEnabled=enabled;
	recordingStart=start;
	recordingEnd=end;
}

void ResultFilter::setLengthRange(bool enabled, int start, int end)
{
	lengthEnabled=enabled;
	lengthStart=start;
	lengthEnd=end;
}

void ResultFilter::setEnergyRange(bool enabled, int start, int end)
{
	energyEnabled=enabled;
	energyStart=start;
	energyEnd=end;
}

void ResultFilter::setMusicKey(bool enabled, int key)
{
	musicKeyEnabled=enabled;
	musicKey=key;
}

void ResultFilter::setGenres(bool enabled, const ppl6::CString &genres)
{
	genresEnabled=enabled;
	genreSet.clear();
	// genres nach Komma splitten
	ppl6::CArray gens(genres,",");

	CTableStore::IndexTree Result;
	CTableStore::IndexTree::const_iterator it;

	for (int i=0;i<gens.Num();i++) {
		wm_main->GenreStore.findWords(Result,gens[i]);
		for (it=Result.begin();it!=Result.end();++it) {
			genreSet.insert(*it);
		}
	}
	/*
	printf ("Genres: \n");
	for (it=genreSet.begin();it!=genreSet.end();++it) {
		CSimpleTable *tab=wm_main->GenreStore.Get(*it);
		if (tab) printf ("id=%i, text=%s\n",*it, tab->Value);
	}
	*/
}

bool ResultFilter::pass(const DataTitle &ti) const
{
	if (bpmEnabled==true && passBpm(ti)==false) return false;
	if (ratingEnabled==true && passRating(ti)==false) return false;
	if (yearEnabled==true && passYear(ti)==false) return false;
	if (recordingDateEnabled==true && passRecordingDate(ti)==false) return false;
	if (musicKeyEnabled==true && passMusicKey(ti)==false) return false;
	if (genresEnabled==true && passGenres(ti)==false) return false;
	if (lengthEnabled==true && passLength(ti)==false) return false;
	if (energyEnabled==true && passEnergyLevel(ti)==false) return false;
	return true;
}

bool ResultFilter::pass(ppluint32 titleId) const
{
	DataTitle *ti=wm_main->GetTitle(titleId);
	if (!ti) return false;
	return pass(*ti);
}

bool ResultFilter::passBpm(const DataTitle &ti) const
{
	if (ti.BPM>=bpmStart && ti.BPM<=bpmEnd) return true;
	return false;
}

bool ResultFilter::passRating(const DataTitle &ti) const
{
	if (ti.Rating>=ratingStart && ti.Rating<=ratingEnd) return true;
	return false;
}

bool ResultFilter::passYear(const DataTitle &ti) const
{
	if (ti.ReleaseDate>=yearStart && ti.ReleaseDate<=yearEnd) return true;
	return false;
}

bool ResultFilter::passRecordingDate(const DataTitle &ti) const
{
	if (ti.RecordDate>=recordingStart && ti.RecordDate<=recordingEnd) return true;
	return false;

}

bool ResultFilter::passMusicKey(const DataTitle &ti) const
{
	if (ti.Key==musicKey) return true;
	return false;
}

bool ResultFilter::passGenres(const DataTitle &ti) const
{
	std::set<ppluint32>::iterator it=genreSet.find(ti.GenreId);
	if (it!=genreSet.end()) return true;
	return false;
}

bool ResultFilter::passLength(const DataTitle &ti) const
{
	if (ti.Length>=lengthStart && ti.Length<=lengthEnd) return true;
	return false;

}

bool ResultFilter::passEnergyLevel(const DataTitle &ti) const
{
	if (ti.EnergyLevel>=energyStart && ti.EnergyLevel<=energyEnd) return true;
	return false;

}
