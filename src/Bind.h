#ifndef _BIND_
#define _BIND_

#include "Dict.h"

class Bind
{
 public:
  Bind();
  ~Bind();

  int BindProc(char *lm_file, char *bd_file);

 private:
  int ReadLmFile(char *lm_file);
  int WriteBdFile(char *bd_file);

  void CalDictBind();
  float GetWordBind(const dict_t *pBdWord);
  float GetBiBind(const dict_t *pBdWord);
  float GetTriBind(const dict_t *pBdWord);
  float GetQuaBind(const dict_t *pBdWord);

  CDictionary *m_pLm;
  dict_t *m_pLmWords;
  int m_nLmWordsNum;

  CDictionary *m_pBd;
  dict_t *m_pBdWords;
  int m_nBdWordsNum;
};

#endif


