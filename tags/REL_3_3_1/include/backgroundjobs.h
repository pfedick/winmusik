/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author$
 * $Revision$
 * $Date$
 * $Id$
 *
 *
 * Copyright (c) 2013 Patrick Fedick
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

#ifndef BACKGROUNDJOBS_H_
#define BACKGROUNDJOBS_H_

#include <queue>


class BackgroundJobs : private ppl6::CThread
{
	Q_DECLARE_TR_FUNCTIONS(BackgroundJobs)

	private:
		ppl6::CMutex Mutex;

		enum JobType {
			TypeUpdateMp3Tags,
			TypeRenameFile,
			TypeWriteTracklist,
			TypeEncodeMp3File,


		};

		class Job
		{
			public:
				Job(JobType type, const ppl6::CAssocArray &data) {
					this->Type=type;
					this->Data=data;
					ErrorCount=0;
				}
				JobType				Type;
				ppl6::CAssocArray	Data;
				int					ErrorCount;
				ppl6::CString		LastError;
		};
		ppluint64 JobIdCount;

		std::queue<Job>	Queue;
		std::queue<Job>	Errors;

	public:
		BackgroundJobs();
		~BackgroundJobs();

		virtual void ThreadMain(void *param);

		void add(JobType type, const ppl6::CAssocArray &data);
		void shutdown();
		void triggerShutdown();
		size_t size() const;
		bool empty() const;

		void updateMp3Tags(const ppl6::CAssocArray &data);
		void renameFile(const ppl6::CAssocArray &data);
		void writeTracklists(const ppl6::CAssocArray &data);


};


#endif /* BACKGROUNDJOBS_H_ */
