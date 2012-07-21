/***************************************************************************
* Copyright (C) 2012,  Gfif Developers                                     *
* Grupo de Fenomenologia de Interacciones Fundamentales                    *
* http://gfif.udea.edu.co                                                  *
* División de ciencias de la computación Gfifdev                           *
* http://gfifdev.udea.edu.co                                               *
* Class Created and Maintained By Omar Andres Zapata Mesa                  *
* All rights reserved.                                                     *
* NOTE: this sode was based in a Qt Example and modified under BSD license *                                                                       *
*                                                                          *
* For the list of contributors see $ROOTSYS/README/CREDITS.                *
****************************************************************************/

#ifndef ROOT_Parallel_Gui_Macro_Highlighter
#define ROOT_Parallel_Gui_Macro_Highlighter

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif


#ifdef __CINT__
class QSyntaxHighlighter;
class QTextDocument;
#else
#include<QTextEdit>
#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>
#endif


namespace ROOT {

   class ParallelGuiMacroHighlighter  : public QSyntaxHighlighter {
      Q_OBJECT

   public:
      ParallelGuiMacroHighlighter(QTextDocument *parent = 0);

   protected:
      void highlightBlock(const QString &text);

   private:
      struct HighlightingRule {
         QRegExp pattern;
         QTextCharFormat format;
      };
      QVector<HighlightingRule> highlightingRules;

      QRegExp commentStartExpression;
      QRegExp commentEndExpression;

      QTextCharFormat keywordFormat;
      QTextCharFormat classFormat;
      QTextCharFormat singleLineCommentFormat;
      QTextCharFormat multiLineCommentFormat;
      QTextCharFormat quotationFormat;
      QTextCharFormat functionFormat;
   };

}

#endif
