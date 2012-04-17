/** @file
	@brief Header

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

#pragma once
#ifndef INCLUDED_QTCodeInput_h_GUID_A5D9CF75_0636_48B3_43C1_A12C02790903
#define INCLUDED_QTCodeInput_h_GUID_A5D9CF75_0636_48B3_43C1_A12C02790903


#define QT_NO_KEYWORDS

// Internal Includes
// - none

// Library/third-party includes
#include <QPlainTextEdit>
#include <QUrl>

// Standard includes
// - none

namespace vrjLua {


	class QTCodeInput : public QPlainTextEdit {
			Q_OBJECT

		public:
			QTCodeInput(QWidget * parent = NULL) : QPlainTextEdit(parent) {}
			virtual ~QTCodeInput();

		Q_SIGNALS:
			/// @brief emitted when a jconf file is dropped
			void gotJconf(QUrl url);

			/// @brief emitted when a lua file is dropped
			void gotLuaFile(QUrl url);
		protected:
			virtual void dragEnterEvent(QDragEnterEvent * e);
			virtual void dragMoveEvent(QDragMoveEvent * e);
			virtual void dropEvent(QDropEvent * e);

		private:
			static bool isJconf(QUrl const& url);
			static bool isLocalLuaFile(QUrl const& url);
	};

} // end of namespace vrjLua

#endif // INCLUDED_QTCodeInput_h_GUID_A5D9CF75_0636_48B3_43C1_A12C02790903

