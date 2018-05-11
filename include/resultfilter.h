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

#ifndef RESULTFILTER_H_
#define RESULTFILTER_H_

#include <ppl6.h>
#include <set>

class ResultFilter
{
	private:
		bool bpmEnabled;
		bool yearEnabled;
		bool recordingDateEnabled;
		bool musicKeyEnabled;
		bool genresEnabled;
		bool ratingEnabled;
		bool lengthEnabled;
		bool energyEnabled;
        bool tracksWithFilesOnlyEnabled;

		ppluint32 bpmStart, bpmEnd;
		ppluint32 yearStart, yearEnd;
		ppluint32 recordingStart, recordingEnd;
		ppluint8 musicKey;
		ppluint8 ratingStart, ratingEnd;
		ppluint32 lengthStart, lengthEnd;
		ppluint8 energyStart, energyEnd;
		typedef std::set<ppluint32> IndexTree;

		IndexTree genreSet;



		bool passBpm(const DataTitle &ti) const;
		bool passYear(const DataTitle &ti) const;
		bool passRecordingDate(const DataTitle &ti) const;
		bool passMusicKey(const DataTitle &ti) const;
		bool passEnergyLevel(const DataTitle &ti) const;
		bool passLength(const DataTitle &ti) const;
		bool passGenres(const DataTitle &ti) const;
		bool passRating(const DataTitle &ti) const;
        bool passTrackWithFile(const DataTitle &ti) const;

	public:
		ResultFilter();
		void disableAll();
		void setBpmRange(bool enabled, int start=0, int end=999);
		void setYearRange(bool enabled, int start=0, int end=9999);
		void setLengthRange(bool enabled, int start=0, int end=9999);
		void setEnergyRange(bool enabled, int start=0, int end=10);
		void setRatingRange(bool enabled, int start=0, int end=6);
		void setRecordingRange(bool enabled, int start=0, int end=99999999);
		void setMusicKey(bool enabled, int key=0);
		void setGenres(bool enabled, const ppl6::CString &genres=ppl6::CString());
        void setTracksWithFilesOnly(bool enabled);
		bool pass(const DataTitle &ti) const;
		bool pass(ppluint32 titleId) const;
};


#endif /* RESULTFILTER_H_ */
