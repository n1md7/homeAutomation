#ifndef CUSTOM_CHARACTERS_H
#define CUSTOM_CHARACTERS_H

extern byte a[8];
extern byte C[8];
extern byte r[8];
extern byte T[8];
extern byte u[8];
extern byte l[8];
extern byte i[8];

// Define custom characters(Georgian letters) (Pixel by pixel)
byte C[8] = {B11110, B10001, B10010, B11100, B10000, B11110, B10001, B10001}; // ჩ
byte a[8] = {B00100, B00100, B00010, B00001, B00001, B10001, B10001, B01110}; // ა
byte r[8] = {B10000, B10000, B10000, B10000, B11110, B10001, B10001, B10001}; // რ
byte T[8] = {B01010, B10101, B10101, B10101, B10101, B10101, B10101, B01001}; // თ
byte u[8] = {B11011, B00101, B00001, B00001, B00001, B10001, B10001, B01110}; // უ
byte l[8] = {B01110, B10001, B10001, B10000, B10000, B01000, B10100, B01111}; // ლ
byte i[8] = {B01110, B10001, B10001, B10001, B10001, B10001, B10001, B10001}; // ი

#endif // CUSTOM_CHARACTERS_H
