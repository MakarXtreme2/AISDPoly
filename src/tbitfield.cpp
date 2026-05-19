// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
  if (len < 0)
    throw -1;
  BitLen = len;
  MemLen = (len + sizeof(TELEM) * 8 - 1) >> (sizeof(TELEM) + 1);
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++)
    pMem[i] ^= pMem[i];
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  for (int i = 0; i < MemLen; i++)
    pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
  delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n >> (sizeof(TELEM) + 1);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n & ((1 << (sizeof(TELEM) + 1)) - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  if (n < 0 || n >= BitLen)
    throw -1;
  pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
  if (n < 0 || n >= BitLen)
    throw -1;
  pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  cout << n << endl;
  if (n < 0 || n >= BitLen)
    throw -1;
  return (pMem[GetMemIndex(n)] & GetMemMask(n)) == 0 ? 0 : 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  if (this == &bf)
    return *this;
  delete[] pMem;
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++)
    pMem[i] = bf.pMem[i];
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if (BitLen != bf.BitLen)
    return 0;
  for (int i = 0; i < MemLen; i++)
    if (pMem[i] != bf.pMem[i])
      return 0;
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  if (*this == bf)
    return 0;
  else
    return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  int n, k, l;
  bool f1 = true;
  if (BitLen >= bf.BitLen)
  {
    n = MemLen;
    k = bf.MemLen;
    l = BitLen;
    f1 = true;
  }
  else
  {
    n = bf.MemLen;
    k = MemLen;
    l = bf.BitLen;
    f1 = false;
  }
  TBitField tmp(l);
  for (int i = 0; i < k; i++)
    tmp.pMem[i] = pMem[i] | bf.pMem[i];
  if (f1)
    for (int i = k; i < n; i++)
      tmp.pMem[i] = pMem[i];
  else
    for (int i = k; i < n; i++)
      tmp.pMem[i] = bf.pMem[i];
  return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
  int k, l;
  if (BitLen >= bf.BitLen)
  {
    k = bf.MemLen;
    l = BitLen;
  }
  else
  {
    k = MemLen;
    l = bf.BitLen;
  }
  TBitField tmp(l);
  for (int i = 0; i < k; i++)
    tmp.pMem[i] = pMem[i] & bf.pMem[i];
  return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField tmp(BitLen);
  for (int i = 0; i < MemLen - 1; i++)
    tmp.pMem[i] = ~pMem[i];
  tmp.pMem[MemLen - 1] = ~pMem[MemLen - 1] & GetMemMask(BitLen) - 1;
  return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  int n, tmp;
  istr >> n;
  for (int i = 0; i < n; i++)
  {
    istr >> tmp;
    bf.SetBit(tmp);
  }
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  ostr << bf.BitLen << std::endl;
  for (int i = 0; i < bf.BitLen; i++)
  {
    if (bf.GetBit(i))
      ostr << i << " ";
  }
  ostr << std::endl;
  return ostr;
}
