/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#ifndef _GUI_TEXT_BOX_HPP_
#define _GUI_TEXT_BOX_HPP_

#include <memory>
#include "../Common/GUI/GUIOverlay.hpp"
#include "../Common/Input/GameInputListener.hpp"
#include "../Common/PauseManager/PauseCaller.hpp"

class CGUITextBox : public CGUIOverlay, public CGameInputListener, public CPauseCaller {
public:
  enum EStatus {
    NONE,
    WAITING,
    QUESTION,
    REVEAL,
  };
  enum EResultTypes {
    RESULT_NONE = 0,
    RESULT_REPEAT,
    RESULT_CONTINUE,
  };
  struct SResult {
    EResultTypes mResult;
    std::mutex mMutex;

  };
private:
  std::shared_ptr<CGUITextBox::SResult> mResult;
  CEGUI::Window *mTextWindow;
  EStatus mStatus;
  Ogre::Real mTimer;

  CEGUI::String mOriginalText;
  CEGUI::String mCompleteText;
  CEGUI::String mTextToDisplay;
  CEGUI::String mTextToAdd;
  unsigned int mCurrentRowToWrite;
  unsigned int mRowsWritten;
  unsigned int mDisplayedRows;
  unsigned int mNextCharacterCounter;
  unsigned int mCurrentLineSize;
  bool mCursorShown;
public:
  CGUITextBox(const std::string &id,
              CEntity *pParentEntity,
              CEGUI::Window *pParentWindow,
              const CEGUI::String &unparsedText,
              std::shared_ptr<CGUITextBox::SResult> result);

  ~CGUITextBox();

  void update(Ogre::Real tpf);

private:
  void popFrontLine();
  void stripCursor();
  void showNextCharacter();
  int nextWordLength();
  bool lineFull();

  void onResume();

  void receiveInputCommand(const CGameInputCommand &cmd);
};

#endif // _GUI_TEXT_BOX_HPP_
