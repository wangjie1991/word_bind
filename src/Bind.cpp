#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "FindLetterAndNumber.h"
#include "Bind.h"

#define STEP 2

const float uni_min = -10.0f;
const float bi_min = -20.0f;


Bind::Bind()
{
  m_pLm = NULL;
  m_pLmWords = NULL;
  m_nLmWordsNum = 0;

  m_pBd = NULL;
  m_pBdWords = NULL;
  m_nBdWordsNum = 0;
}

Bind::~Bind()
{
  if (m_pLm)
    delete m_pLm;
  if (m_pBd)
    delete m_pBd;
}

int Bind::BindProc(char *lm_file, char *bd_file)
{
  if (0 != ReadLmFile(lm_file))
  {
    return -1;
  }

  CalDictBind();

  if (0 != WriteBdFile(bd_file))
  {
    return -1;
  }

  return 0;
}

int Bind::ReadLmFile(char *lm_file)
{
  if (m_pLm)
    delete m_pLm;
  if (m_pBd)
    delete m_pBd;

  m_pLm = new CDictionary();
  if (0 != m_pLm->Init(lm_file))
  {
    delete m_pLm;
    return -1;
  }
  m_pLmWords = m_pLm->GetDict();
  m_nLmWordsNum = m_pLm->GetDictWordNum();

  m_pBd = new CDictionary();
  if (0 != m_pBd->Init(lm_file))
  {
    delete m_pBd;
    return -1;
  }
  m_pBdWords = m_pBd->GetDict();
  m_nBdWordsNum = m_pBd->GetDictWordNum();

  return 0;
}

int Bind::WriteBdFile(char *bd_file)
{
  FILE* fp = fopen(bd_file, "w");
  if (fp == NULL)
  {
    printf("can't open %s\n", bd_file);
    return -1;
  }

  char szLine[1024];
  char szWeight[20];
  for (int i = 0; i < m_nBdWordsNum; i++)
  {
    szLine[0] = 0;
    dict_t *pWord = &m_pBdWords[i];

    strcat(szLine, pWord->word);
    strcat(szLine, "\t");

    sprintf(szWeight, "%f", pWord->fWeight);
    strcat(szLine, szWeight);

    fprintf(fp, "%s\n", szLine);
  }

  fclose(fp);

  return 0;
}

void Bind::CalDictBind()
{
  for (int i = 0; i < m_nBdWordsNum; i++)
  {
    dict_t *pBdWord = &m_pBdWords[i];
    pBdWord->fWeight = GetWordBind(pBdWord);
  }

  return;
}

float Bind::GetWordBind(const dict_t *pBdWord)
{
  /* if not chinese charactor */
  if (STEP != JustGetStepSize(pBdWord->word))
  {
    return 0;
  }

  float fBind = 0;
  switch (pBdWord->nElem)
  {
    case 2:
    {
      fBind = GetBiBind(pBdWord);
      break;
    }
    case 3:
    {
      fBind = GetTriBind(pBdWord);
      break;
    }
    case 4:
    {
      fBind = GetQuaBind(pBdWord);
      break;
    }
    default:
    {
      fBind = 0;
      break;
    }
  }

  return fBind;
}

float Bind::GetBiBind(const dict_t *pBdWord)
{
  float fWord = 0;
  int nIndex = m_pLm->Find(pBdWord->word);
  /* if not found, set weight to default value */
  if (nIndex < 0)
  {
    return 0;
  }
  else
  {
    fWord = m_pLmWords[nIndex].fWeight;
  }

  /**
   * bind = P(w1 w2)/(P(w1) * P(w2))
   * weight = log(P)
   * log(bind) = weight(w1 w2) - (weight(w1) + weight(w2))
   */
  float fSum = 0;
  for (int i = 0; i < 2; i++)
  {
    char szChar[STEP+1];
    char* ch = pBdWord->word + (i * STEP);
    memcpy(szChar, ch, STEP);
    szChar[STEP] = 0;

    /* Search single charactor */
    int id = m_pLm->Find(szChar);
    if (id >= 0)
    {
      fSum += m_pLmWords[id].fWeight;
    }
    else
    {
      fSum += uni_min;
    }
  }

  return (fWord - fSum);
}

float Bind::GetTriBind(const dict_t *pBdWord)
{
  float fWord = 0;
  int nIndex = m_pLm->Find(pBdWord->word);
  /* if not found, set weight to default value */
  if (nIndex < 0)
  {
    return 0;
  }
  else
  {
    fWord = m_pLmWords[nIndex].fWeight;
  }

  float fSum = 0;
  float fSum1 = 0;
  float fSum2 = 0;
  float fSum3 = 0;
  int id = 0;
  char* ch = NULL;
  char szChar[2*STEP+1];

  /* log(bind) = weight(w1 w2 w3) - (weight(w1) + weight(w2 w3)) */
  ch = pBdWord->word;
  memcpy(szChar, ch, STEP);
  szChar[STEP] = 0;

  id = m_pLm->Find(szChar);
  if (id >= 0)
  {
    fSum1 += m_pLmWords[id].fWeight;
  }
  else
  {
    fSum1 += uni_min;
  }

  ch = pBdWord->word + STEP;
  memcpy(szChar, ch, 2*STEP);
  szChar[2*STEP] = 0;

  id = m_pLm->Find(szChar);
  if (id >= 0)
  {
    fSum1 += m_pLmWords[id].fWeight;
  }
  else
  {
    fSum1 += bi_min;
  }
  
  /* log(bind) = weight(w1 w2 w3) - (weight(w1 w2) + weight(w3)) */
  ch = pBdWord->word;
  memcpy(szChar, ch, 2*STEP);
  szChar[2*STEP] = 0;

  id = m_pLm->Find(szChar);
  if (id >= 0)
  {
    fSum2 += m_pLmWords[id].fWeight;
  }
  else
  {
    fSum2 += bi_min;
  }

  ch = pBdWord->word + (2*STEP);
  memcpy(szChar, ch, STEP);
  szChar[STEP] = 0;

  id = m_pLm->Find(szChar);
  if (id >= 0)
  {
    fSum2 += m_pLmWords[id].fWeight;
  }
  else
  {
    fSum2 += uni_min;
  }

  /** 
   * log(bind) = weight(w1 w2 w3) - 
   *             (weight(w1) + weight(w2) + weight(w3)) 
   */
  for (int i = 0; i < 3; i++)
  {
    ch = pBdWord->word + (i * STEP);
    memcpy(szChar, ch, STEP);
    szChar[STEP] = 0;

    int id = m_pLm->Find(szChar);
    if (id >= 0)
    {
      fSum3 += m_pLmWords[id].fWeight;
    }
    else
    {
      fSum3 += uni_min;
    }
  }

  /* Get max value */
  fSum = ((fSum1 > ((fSum2 > fSum3)? fSum2:fSum3))? fSum1:
          ((fSum2 > fSum3)? fSum2:fSum3));

  return (fWord - fSum);
}

float Bind::GetQuaBind(const dict_t *pBdWord)
{
  float fWord = 0;
  int nIndex = m_pLm->Find(pBdWord->word);
  /* if not found, set weight to default value */
  if (nIndex < 0)
  {
    return 0;
  }
  else
  {
    fWord = m_pLmWords[nIndex].fWeight;
  }

  float fSum = 0;
  float fSum1 = 0;
  float fSum2 = 0;
  /* log(bind) = weight(w1 w2 w3 w4) - (weight(w1 w2) + weight(w3 w4)) */
  for (int i = 0; i < 2; i++)
  {
    char szChar[2*STEP+1];
    char* ch = pBdWord->word + (i * 2 * STEP);
    memcpy(szChar, ch, 2*STEP);
    szChar[2*STEP] = 0;

    /* Search single charactor */
    int id = m_pLm->Find(szChar);
    if (id >= 0)
    {
      fSum1 += m_pLmWords[id].fWeight;
    }
    else
    {
      fSum1 += bi_min;
    }
  }

  /** 
   * log(bind) = weight(w1 w2 w3 w4) - 
   *             (weight(w1) + weight(w2) + weight(w3) + weight(w4))  
   */
  for (int i = 0; i < 4; i++)
  {
    char szChar[STEP+1];
    char* ch = pBdWord->word + (i * STEP);
    memcpy(szChar, ch, STEP);
    szChar[STEP] = 0;

    int id = m_pLm->Find(szChar);
    if (id >= 0)
    {
      fSum2 += m_pLmWords[id].fWeight;
    }
    else
    {
      fSum2 += uni_min;
    }
  }

  fSum = ((fSum1 > fSum2)? fSum1:fSum2);

  return (fWord - fSum);
}


