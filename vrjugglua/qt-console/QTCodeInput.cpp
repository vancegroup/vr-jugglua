/** @file
	@brief Implementation

	@date 2012

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program

*/

//           Copyright Iowa State University 2012.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

// Internal Includes
#include "QTCodeInput.h"

// Library/third-party includes
// - none


// Standard includes
// - none

namespace vrjLua {
	QTCodeInput::~QTCodeInput() {

	}

	void QTCodeInput::dragEnterEvent(QDragEnterEvent * e) {
		if (e->mimeData()->hasUrls()) {

			QList<QUrl> urlList = e->mimeData()->urls();
			for (int i = 0; i < urlList.size(); ++i) {
				/// @TODO handle files that aren't jconfs or lua files by just putting their name.
				if (isJconf(urlList.at(i)) || isLocalLuaFile(urlList.at(i))) {
					e->acceptProposedAction();
					break;
				}
			}
		} else {
			QPlainTextEdit::dragEnterEvent(e);
		}
	}
	void QTCodeInput::dragMoveEvent(QDragMoveEvent * e) {
		if (e->mimeData()->hasUrls()) {

			QList<QUrl> urlList = e->mimeData()->urls();
			for (int i = 0; i < urlList.size(); ++i) {
				if (isJconf(urlList.at(i)) || isLocalLuaFile(urlList.at(i))) {
					e->acceptProposedAction();
					break;
				}
			}
		} else {
			QPlainTextEdit::dragMoveEvent(e);
		}

	}
	void QTCodeInput::dropEvent(QDropEvent * e) {
		if (e->mimeData()->hasUrls()) {
			QList<QUrl> urlList = e->mimeData()->urls();
			bool accept = false;
			for (int i = 0; i < urlList.size(); ++i) {
				QUrl current = urlList.at(i);
				if (isJconf(current)) {
					accept = true;
					Q_EMIT gotJconf(current);
				} else if (isLocalLuaFile(current)) {
					accept = true;
					Q_EMIT gotLuaFile(current);
				}
			}
			if (accept) {
				e->acceptProposedAction();
			}
		} else {
			QPlainTextEdit::dropEvent(e);
		}
	}
	bool QTCodeInput::isJconf(QUrl const& url) {
		return /*url.isLocalFile() url.scheme() == "file" && */url.toLocalFile().endsWith(QString(".jconf"), Qt::CaseInsensitive);
	}
	bool QTCodeInput::isLocalLuaFile(QUrl const& url) {
		return /*url.isLocalFile() url.scheme() == "file" && */url.toLocalFile().endsWith(QString(".lua"), Qt::CaseInsensitive);
	}
} // end of namespace vrjLua