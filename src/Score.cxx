/*
 * Score.cxx
 * Daniel Nelson - 12/3/1
 *
 * Copyright (C) 2000  Daniel Nelson
 * Copyright (C) 2004  Andrew Sayman
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Daniel Nelson - aluminumangel.org
 * 174 W. 18th Ave.
 * Columbus, OH  43210
 *
 * Handles the score for solo games.
 */

#include <cstring>
#include <fstream>

using namespace std;

#include "TextureLoader.h"
#include "Game.h"
#include "MetaState.h"
#include "Score.h"

int Score::score;
int Score::backlog;
int Score::top_combo_multiplier;
int Score::top_combo_score;
ComboRank Score::combo_record[GC_SCORE_MULT_LENGTH];
short Score::digits[GC_NUMBER_DIGITS];
short Score::previous_digits[GC_NUMBER_DIGITS];
int Score::fade_timer;
GLfloat Score::inverse_timer_start;
int Score::n_digits_displayed;
int Score::special_block_scores[BF_NUMBER_SPECIAL]
 = { 30,    // black
     30,    // white
     5,     // special purple
     5,     // special blue
     8,     // special green
     15,    // special yellow
     10, }; // special orange

int Score::player_rank;
Rank Score::record[GC_SCORE_REC_LENGTH];

void Score::initialize (   )
{
  if (!(MetaState::mode & CM_SOLO)) return;

  score = backlog = top_combo_multiplier = top_combo_score = 0;
  n_digits_displayed = GC_MIN_NUMBER_DIGITS_DISPLAYED;
  for (int n = GC_NUMBER_DIGITS; n--; ) {
    digits[n] = previous_digits[n] = 0;
    fade_timer = 0;
    inverse_timer_start = 1.0f;
  }
  player_rank = -1;

  if (!readScoreRecord()) setupDefaultScoreRecord();
  if (!readMultRecord()) setupDefaultMultRecord();
}

void Score::cleanUp (   )
{
  if ((MetaState::mode & CM_SOLO) && player_rank != -1)
    writeScoreRecord();
}

int Score::gameFinish (   )
{
  // first do non-victory deciding scores
  for (int n = GC_SCORE_MULT_LENGTH; n--; )
    if (top_combo_multiplier > combo_record[n].multiplier) {
      player_rank = n;

      // insert player
      for (int n = 0; n < player_rank; n++) {
        strncpy(combo_record[n].name, combo_record[n + 1].name, GC_PLAYER_NAME_LENGTH);
        combo_record[n].multiplier = combo_record[n + 1].multiplier;
      }
      strncpy(combo_record[player_rank].name, MetaState::player_name,
       GC_PLAYER_NAME_LENGTH);
      combo_record[player_rank].multiplier = top_combo_multiplier;
      break;
    }

  // now decide the winner
  for (int n = GC_SCORE_REC_LENGTH; n--; )
    if (score > record[n].score) {
      player_rank = n;

      // insert player
      for (int n = 0; n < player_rank; n++) {
        strncpy(record[n].name, record[n + 1].name, GC_PLAYER_NAME_LENGTH);
        record[n].score = record[n + 1].score;
      }
      strncpy(record[player_rank].name, MetaState::player_name,
       GC_PLAYER_NAME_LENGTH);
      record[player_rank].score = score;

      return GS_WON;
    }
  return GS_LOST;
}

bool Score::readMultRecord (   )
{
  char file_name[256];
  char buffer[256];
  TextureLoader::buildLocalDataFileName(GC_MULT_FILE_NAME, file_name);
  ifstream new_file(file_name);
  if (new_file.fail()) return false;

  for (int n = GC_SCORE_MULT_LENGTH; n--; ) {
    new_file.getline(combo_record[n].name, GC_PLAYER_NAME_LENGTH);
    new_file.getline(buffer, 256);
    combo_record[n].multiplier = atoi(buffer);
    if (new_file.fail()) return false;
  }
  return true;
}

bool Score::readScoreRecord (   )
{
  char file_name[256];
  TextureLoader::buildLocalDataFileName((MetaState::mode & CM_X)
   ? GC_X_REC_FILE_NAME : GC_REC_FILE_NAME, file_name);
  ifstream file(file_name);
  if (file.fail()) return false;

  char buffer[256];
  for (int n = GC_SCORE_REC_LENGTH; n--; ) {
    file.getline(record[n].name, GC_PLAYER_NAME_LENGTH);
    file.getline(buffer, 256);
    record[n].score = atoi(buffer);
    if (file.fail()) return false;
  }
  return true;
}

void Score::writeScoreRecord (   )
{
  char file_name[256];
  TextureLoader::buildLocalDataFileName((MetaState::mode & CM_X)
   ? GC_X_REC_FILE_NAME : GC_REC_FILE_NAME, file_name);
  ofstream file(file_name);
  if (file.fail()) {
    cerr << "Error writing to score record file '" << file_name << "'." << endl;
    exit(1);
  }

  for (int n = GC_SCORE_REC_LENGTH; n--; )
    file << record[n].name << '\n' << record[n].score << '\n';
  file.close();

  // mult record
  TextureLoader::buildLocalDataFileName(GC_MULT_FILE_NAME, file_name);
  ofstream mult(file_name);
  if (mult.fail()) {
    cerr << "Error writing to score record file '" << file_name << "'." << endl;
    exit(1);
  }
  for (int n = GC_SCORE_MULT_LENGTH; n--; )
    mult << combo_record[n].name << '\n' << combo_record[n].multiplier << '\n';
  mult.close();
}


void Score::setupDefaultMultRecord (   )
{
  ifstream new_file(GC_DEFAULT_MULT_FILE_NAME);
  if (new_file.fail()) {
    cerr << "Error opening data file '" << GC_DEFAULT_MULT_FILE_NAME << "'." << endl;
    exit(1);
  }

  char buffer[256];
  for (int n = GC_SCORE_MULT_LENGTH; n--; ) {
    new_file.getline(combo_record[n].name, GC_PLAYER_NAME_LENGTH);
    new_file.getline(buffer, 256);
    combo_record[n].multiplier = atoi(buffer);
    if (new_file.fail()) break;
  }
  writeScoreRecord();
}

void Score::setupDefaultScoreRecord (   )
{
  ifstream file(GC_DEFAULT_REC_FILE_NAME);
  if (file.fail()) {
    cerr << "Error opening data file '" << GC_DEFAULT_REC_FILE_NAME << "'." << endl;
    exit(1);
  }

  int n;
  for (n = GC_SCORE_REC_LENGTH; n--; ) {
    file.getline(record[n].name, GC_PLAYER_NAME_LENGTH);
    if (file.fail()) break;
  }
  if (n != -1)
    while (n--)
      strncpy(record[n].name, GC_SCORE_REC_DEFAULT_NAME,
       GC_PLAYER_NAME_LENGTH);
  for (n = GC_SCORE_REC_LENGTH; n--; )
    record[n].score
     = ((n + 1) * GC_SCORE_DEFAULT_TOP_SCORE) / GC_SCORE_REC_LENGTH;

  writeScoreRecord();
}
