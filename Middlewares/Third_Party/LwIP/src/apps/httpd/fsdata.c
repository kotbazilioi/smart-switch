#include "fs.h"
#include "lwip/def.h"
#include "fsdata.h"


#define file_NULL (struct fsdata_file *) NULL


static const unsigned int dummy_align__img_netping_gif = 0;
static const unsigned char data__img_netping_gif[] = {
/* /img/netping.gif (17 chars) */
0x2f,0x69,0x6d,0x67,0x2f,0x6e,0x65,0x74,0x70,0x69,0x6e,0x67,0x2e,0x67,0x69,0x66,
0x00,0x00,0x00,0x00,

/* HTTP header */
/* "HTTP/1.1 200 OK
" (17 bytes) */
0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x31,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,
0x0a,
/* "Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
" (63 bytes) */
0x53,0x65,0x72,0x76,0x65,0x72,0x3a,0x20,0x6c,0x77,0x49,0x50,0x2f,0x31,0x2e,0x33,
0x2e,0x31,0x20,0x28,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x73,0x61,0x76,0x61,0x6e,
0x6e,0x61,0x68,0x2e,0x6e,0x6f,0x6e,0x67,0x6e,0x75,0x2e,0x6f,0x72,0x67,0x2f,0x70,
0x72,0x6f,0x6a,0x65,0x63,0x74,0x73,0x2f,0x6c,0x77,0x69,0x70,0x29,0x0d,0x0a,
/* "Content-Length: 3128
" (18+ bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x4c,0x65,0x6e,0x67,0x74,0x68,0x3a,0x20,
0x33,0x31,0x32,0x38,0x0d,0x0a,
/* "Connection: Close
" (19 bytes) */
0x43,0x6f,0x6e,0x6e,0x65,0x63,0x74,0x69,0x6f,0x6e,0x3a,0x20,0x43,0x6c,0x6f,0x73,
0x65,0x0d,0x0a,
/* "Content-type: image/gif

" (27 bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x74,0x79,0x70,0x65,0x3a,0x20,0x69,0x6d,
0x61,0x67,0x65,0x2f,0x67,0x69,0x66,0x0d,0x0a,0x0d,0x0a,
/* raw file data (3128 bytes) */
0x47,0x49,0x46,0x38,0x39,0x61,0xa3,0x00,0x3b,0x00,0x70,0x00,0x00,0x21,0xf9,0x04,
0x01,0x00,0x00,0xfc,0x00,0x2c,0x00,0x00,0x00,0x00,0xa3,0x00,0x3b,0x00,0x87,0x00,
0x00,0x00,0x00,0x00,0x33,0x00,0x00,0x66,0x00,0x00,0x99,0x00,0x00,0xcc,0x00,0x00,
0xff,0x00,0x2b,0x00,0x00,0x2b,0x33,0x00,0x2b,0x66,0x00,0x2b,0x99,0x00,0x2b,0xcc,
0x00,0x2b,0xff,0x00,0x55,0x00,0x00,0x55,0x33,0x00,0x55,0x66,0x00,0x55,0x99,0x00,
0x55,0xcc,0x00,0x55,0xff,0x00,0x80,0x00,0x00,0x80,0x33,0x00,0x80,0x66,0x00,0x80,
0x99,0x00,0x80,0xcc,0x00,0x80,0xff,0x00,0xaa,0x00,0x00,0xaa,0x33,0x00,0xaa,0x66,
0x00,0xaa,0x99,0x00,0xaa,0xcc,0x00,0xaa,0xff,0x00,0xd5,0x00,0x00,0xd5,0x33,0x00,
0xd5,0x66,0x00,0xd5,0x99,0x00,0xd5,0xcc,0x00,0xd5,0xff,0x00,0xff,0x00,0x00,0xff,
0x33,0x00,0xff,0x66,0x00,0xff,0x99,0x00,0xff,0xcc,0x00,0xff,0xff,0x33,0x00,0x00,
0x33,0x00,0x33,0x33,0x00,0x66,0x33,0x00,0x99,0x33,0x00,0xcc,0x33,0x00,0xff,0x33,
0x2b,0x00,0x33,0x2b,0x33,0x33,0x2b,0x66,0x33,0x2b,0x99,0x33,0x2b,0xcc,0x33,0x2b,
0xff,0x33,0x55,0x00,0x33,0x55,0x33,0x33,0x55,0x66,0x33,0x55,0x99,0x33,0x55,0xcc,
0x33,0x55,0xff,0x33,0x80,0x00,0x33,0x80,0x33,0x33,0x80,0x66,0x33,0x80,0x99,0x33,
0x80,0xcc,0x33,0x80,0xff,0x33,0xaa,0x00,0x33,0xaa,0x33,0x33,0xaa,0x66,0x33,0xaa,
0x99,0x33,0xaa,0xcc,0x33,0xaa,0xff,0x33,0xd5,0x00,0x33,0xd5,0x33,0x33,0xd5,0x66,
0x33,0xd5,0x99,0x33,0xd5,0xcc,0x33,0xd5,0xff,0x33,0xff,0x00,0x33,0xff,0x33,0x33,
0xff,0x66,0x33,0xff,0x99,0x33,0xff,0xcc,0x33,0xff,0xff,0x66,0x00,0x00,0x66,0x00,
0x33,0x66,0x00,0x66,0x66,0x00,0x99,0x66,0x00,0xcc,0x66,0x00,0xff,0x66,0x2b,0x00,
0x66,0x2b,0x33,0x66,0x2b,0x66,0x66,0x2b,0x99,0x66,0x2b,0xcc,0x66,0x2b,0xff,0x66,
0x55,0x00,0x66,0x55,0x33,0x66,0x55,0x66,0x66,0x55,0x99,0x66,0x55,0xcc,0x66,0x55,
0xff,0x66,0x80,0x00,0x66,0x80,0x33,0x66,0x80,0x66,0x66,0x80,0x99,0x66,0x80,0xcc,
0x66,0x80,0xff,0x66,0xaa,0x00,0x66,0xaa,0x33,0x66,0xaa,0x66,0x66,0xaa,0x99,0x66,
0xaa,0xcc,0x66,0xaa,0xff,0x66,0xd5,0x00,0x66,0xd5,0x33,0x66,0xd5,0x66,0x66,0xd5,
0x99,0x66,0xd5,0xcc,0x66,0xd5,0xff,0x66,0xff,0x00,0x66,0xff,0x33,0x66,0xff,0x66,
0x66,0xff,0x99,0x66,0xff,0xcc,0x66,0xff,0xff,0x99,0x00,0x00,0x99,0x00,0x33,0x99,
0x00,0x66,0x99,0x00,0x99,0x99,0x00,0xcc,0x99,0x00,0xff,0x99,0x2b,0x00,0x99,0x2b,
0x33,0x99,0x2b,0x66,0x99,0x2b,0x99,0x99,0x2b,0xcc,0x99,0x2b,0xff,0x99,0x55,0x00,
0x99,0x55,0x33,0x99,0x55,0x66,0x99,0x55,0x99,0x99,0x55,0xcc,0x99,0x55,0xff,0x99,
0x80,0x00,0x99,0x80,0x33,0x99,0x80,0x66,0x99,0x80,0x99,0x99,0x80,0xcc,0x99,0x80,
0xff,0x99,0xaa,0x00,0x99,0xaa,0x33,0x99,0xaa,0x66,0x99,0xaa,0x99,0x99,0xaa,0xcc,
0x99,0xaa,0xff,0x99,0xd5,0x00,0x99,0xd5,0x33,0x99,0xd5,0x66,0x99,0xd5,0x99,0x99,
0xd5,0xcc,0x99,0xd5,0xff,0x99,0xff,0x00,0x99,0xff,0x33,0x99,0xff,0x66,0x99,0xff,
0x99,0x99,0xff,0xcc,0x99,0xff,0xff,0xcc,0x00,0x00,0xcc,0x00,0x33,0xcc,0x00,0x66,
0xcc,0x00,0x99,0xcc,0x00,0xcc,0xcc,0x00,0xff,0xcc,0x2b,0x00,0xcc,0x2b,0x33,0xcc,
0x2b,0x66,0xcc,0x2b,0x99,0xcc,0x2b,0xcc,0xcc,0x2b,0xff,0xcc,0x55,0x00,0xcc,0x55,
0x33,0xcc,0x55,0x66,0xcc,0x55,0x99,0xcc,0x55,0xcc,0xcc,0x55,0xff,0xcc,0x80,0x00,
0xcc,0x80,0x33,0xcc,0x80,0x66,0xcc,0x80,0x99,0xcc,0x80,0xcc,0xcc,0x80,0xff,0xcc,
0xaa,0x00,0xcc,0xaa,0x33,0xcc,0xaa,0x66,0xcc,0xaa,0x99,0xcc,0xaa,0xcc,0xcc,0xaa,
0xff,0xcc,0xd5,0x00,0xcc,0xd5,0x33,0xcc,0xd5,0x66,0xcc,0xd5,0x99,0xcc,0xd5,0xcc,
0xcc,0xd5,0xff,0xcc,0xff,0x00,0xcc,0xff,0x33,0xcc,0xff,0x66,0xcc,0xff,0x99,0xcc,
0xff,0xcc,0xcc,0xff,0xff,0xff,0x00,0x00,0xff,0x00,0x33,0xff,0x00,0x66,0xff,0x00,
0x99,0xff,0x00,0xcc,0xff,0x00,0xff,0xff,0x2b,0x00,0xff,0x2b,0x33,0xff,0x2b,0x66,
0xff,0x2b,0x99,0xff,0x2b,0xcc,0xff,0x2b,0xff,0xff,0x55,0x00,0xff,0x55,0x33,0xff,
0x55,0x66,0xff,0x55,0x99,0xff,0x55,0xcc,0xff,0x55,0xff,0xff,0x80,0x00,0xff,0x80,
0x33,0xff,0x80,0x66,0xff,0x80,0x99,0xff,0x80,0xcc,0xff,0x80,0xff,0xff,0xaa,0x00,
0xff,0xaa,0x33,0xff,0xaa,0x66,0xff,0xaa,0x99,0xff,0xaa,0xcc,0xff,0xaa,0xff,0xff,
0xd5,0x00,0xff,0xd5,0x33,0xff,0xd5,0x66,0xff,0xd5,0x99,0xff,0xd5,0xcc,0xff,0xd5,
0xff,0xff,0xff,0x00,0xff,0xff,0x33,0xff,0xff,0x66,0xff,0xff,0x99,0xff,0xff,0xcc,
0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,
0xff,0x00,0xf7,0x09,0x1c,0x48,0xb0,0xa0,0xc1,0x83,0x08,0x13,0x2a,0x5c,0xc8,0xb0,
0xa1,0xc3,0x87,0x10,0x23,0x4a,0x9c,0x48,0xb1,0xa2,0xc5,0x8b,0x18,0x33,0x6a,0xdc,
0xc8,0xb1,0xa3,0xc7,0x8f,0x20,0x43,0x8a,0x1c,0x49,0xb2,0xa4,0xc9,0x93,0x28,0x53,
0xaa,0x5c,0xc9,0xb2,0xa5,0xcb,0x97,0x30,0x63,0xca,0x9c,0x49,0xb3,0xa6,0xcb,0x22,
0x43,0x88,0x10,0xc9,0x99,0xb3,0x87,0xce,0x9c,0x3f,0x77,0x12,0xf1,0x09,0x74,0x68,
0x50,0x9e,0x46,0x85,0xfe,0xf4,0x81,0x54,0xa9,0x4f,0xa1,0x48,0x7b,0xea,0x24,0x12,
0x6d,0x23,0xb4,0x37,0xfa,0x6c,0x2a,0x8c,0xd6,0x00,0x43,0x03,0x09,0x13,0x1a,0x4c,
0x08,0x9b,0x61,0xac,0xd8,0x09,0x12,0x1c,0x48,0xa0,0xd0,0xa0,0xec,0x58,0xb2,0x67,
0xcf,0xc2,0xcd,0x20,0xf6,0x2b,0x85,0xb5,0x6d,0xeb,0x92,0x55,0xcb,0xd6,0xac,0x59,
0xba,0x68,0x1b,0x7c,0xda,0xf8,0x89,0x08,0x1c,0xad,0x08,0xdf,0x6a,0x98,0xb0,0x38,
0x03,0x05,0xb7,0x8d,0x19,0x4f,0x28,0xeb,0x78,0x2c,0x65,0xcb,0x8f,0x27,0x50,0x98,
0x3c,0x79,0x73,0xd9,0xc8,0x8e,0x2f,0x53,0x16,0xdd,0x39,0xb4,0x66,0x0c,0x3d,0x96,
0x6d,0x54,0x76,0x15,0x1a,0xe2,0x82,0x6f,0xdd,0xba,0xcd,0x3c,0x76,0xf3,0x5b,0xcf,
0xb5,0x21,0xc7,0xee,0x6c,0x59,0xb3,0x5b,0xcd,0x9c,0x7d,0xf3,0x7e,0x5b,0x84,0xc8,
0x5b,0xce,0x6e,0x07,0x6b,0x54,0x06,0x2a,0xd9,0x0f,0x37,0xaf,0x05,0x42,0x03,0xbb,
0x38,0x37,0xe7,0xcc,0x9f,0x91,0xe3,0xa6,0x6d,0x1a,0x77,0xe5,0xb2,0xdc,0x7b,0x03,
0xff,0xef,0x3c,0x04,0xd4,0x3e,0x37,0x63,0x17,0x6f,0xce,0x6c,0x5e,0xe3,0x27,0xb0,
0x44,0x94,0xbf,0x8e,0x16,0x3b,0xb3,0x6d,0xdc,0xe2,0x85,0x1f,0x1f,0x7e,0x59,0xbf,
0x6c,0xdf,0xb6,0x45,0x36,0x81,0x0f,0xae,0xed,0x53,0x44,0x6f,0xbf,0x4d,0x20,0xdf,
0x45,0xca,0x6c,0xf6,0x46,0x74,0x03,0xe9,0xd3,0x16,0x6d,0xc8,0xe9,0xb6,0x58,0x75,
0xe0,0x65,0xd0,0x18,0x76,0xf7,0x51,0xd6,0x40,0x75,0x8f,0xe1,0xf7,0x9d,0x64,0xd0,
0x09,0x84,0x5e,0x7a,0xa5,0x4d,0xd0,0x1e,0x46,0x6f,0xbc,0x51,0x20,0x84,0xfb,0xd0,
0x77,0x1b,0x78,0xc2,0x51,0x58,0xe3,0x6c,0x34,0xa6,0x98,0x81,0x03,0x94,0xd9,0x98,
0x5d,0x82,0x13,0xfc,0x30,0x10,0x7a,0xf6,0x1d,0xb7,0x60,0x45,0xfa,0x28,0x03,0x63,
0x41,0x0c,0x5c,0x88,0x60,0x70,0xbd,0x09,0xb8,0x9d,0x6e,0x39,0x9a,0x86,0x22,0x8d,
0x9e,0x61,0x47,0xc4,0x40,0xc6,0x8d,0x58,0xdd,0x8a,0x4b,0x5a,0x05,0x96,0x6c,0xeb,
0xd1,0xa5,0x41,0x88,0x63,0x81,0xd5,0xd6,0x75,0x19,0x48,0x70,0xa1,0x58,0x1c,0x02,
0xf6,0x96,0x9b,0x6b,0x5e,0xe6,0x59,0x03,0x68,0x0c,0x54,0x04,0x58,0x18,0x8c,0x57,
0x56,0x26,0x0b,0x81,0x92,0xc9,0x27,0x60,0x66,0x04,0x4d,0x26,0xa0,0x14,0xca,0x10,
0x73,0xa0,0xbc,0x38,0x11,0x57,0x4e,0x8a,0x67,0x5c,0x6c,0x3f,0xbc,0x51,0x87,0x1b,
0x45,0xf8,0x60,0xd9,0x87,0x96,0xd9,0xf9,0x16,0x11,0x2d,0xd6,0xf1,0x06,0x11,0x77,
0xd1,0x85,0x9d,0x64,0x13,0x28,0x31,0xe4,0x66,0x3f,0xf4,0x97,0xc1,0x8a,0x99,0x88,
0xff,0x45,0x84,0x6b,0x6f,0xa0,0x85,0x16,0x03,0x03,0x9a,0x82,0x50,0x26,0x6f,0x69,
0x42,0x10,0x28,0x61,0x4d,0x50,0x95,0x32,0x1a,0x7c,0xd5,0x95,0x04,0x0d,0x50,0xb5,
0x50,0x61,0x5f,0xdd,0xaa,0xa2,0x40,0x6f,0x3d,0xe8,0x90,0x9a,0xa1,0xd1,0xd5,0x1e,
0x34,0x63,0x1d,0x29,0x10,0x28,0x5d,0x8a,0x67,0x1b,0x58,0x8a,0x6e,0xbb,0xdd,0x5b,
0x0a,0x9d,0xf5,0x98,0x60,0x03,0x81,0x22,0x81,0x04,0x6e,0x2c,0x43,0x04,0x03,0xbf,
0x95,0x85,0x2c,0x74,0x55,0x11,0x54,0xeb,0x04,0x0c,0x80,0xa9,0x8c,0xbc,0xfb,0x00,
0x2b,0xc1,0x6f,0x8b,0x81,0x05,0xe8,0x41,0x57,0x81,0x35,0x1c,0x5a,0x3e,0xec,0xd3,
0x03,0x5a,0x03,0x33,0x04,0x4d,0x5b,0xd5,0xf9,0xa6,0x1c,0xb0,0x8e,0x1a,0xa4,0xa9,
0x75,0x6e,0x6d,0xb9,0x10,0x5d,0x19,0x50,0x96,0x70,0x42,0x18,0x70,0x07,0xeb,0x58,
0x3a,0x7d,0x2a,0xaa,0x71,0x8f,0x31,0x70,0x58,0x41,0x9f,0x4c,0xc6,0x80,0xae,0x03,
0x29,0x53,0x5b,0x1d,0xb8,0x8e,0xe5,0x83,0x71,0x13,0xba,0xa9,0xa4,0x41,0x44,0x88,
0x45,0xd7,0x53,0x9a,0x96,0x45,0x44,0x65,0xd2,0x32,0xc4,0xd5,0x7f,0x19,0x30,0x20,
0xed,0x32,0xed,0x45,0xc3,0x1c,0x34,0x3b,0x4b,0x37,0xee,0x58,0x45,0x10,0x04,0x75,
0x1d,0x88,0x3a,0x7a,0xa2,0x6d,0xca,0x74,0x1d,0xe3,0x3e,0xd0,0x80,0xc2,0x1a,0x66,
0x63,0x35,0xfc,0x49,0x82,0x3e,0xd4,0x2b,0x50,0xcb,0x93,0x49,0xd0,0xb0,0x40,0x76,
0x70,0x06,0xa6,0x29,0x0b,0x53,0x46,0xc4,0xdb,0x9f,0xac,0x39,0x81,0xc6,0xf6,0x5a,
0xff,0xd6,0x43,0x1d,0xbf,0x5e,0x9c,0x26,0xe0,0x0d,0x41,0xc3,0x40,0x91,0x65,0x15,
0x9a,0xc9,0x10,0x7e,0x15,0xbd,0xcf,0xbd,0xd9,0xcd,0x3a,0x90,0x26,0x43,0x88,0x05,
0x56,0x0f,0x45,0x47,0x63,0xdb,0x6f,0x25,0xee,0xe3,0x83,0xc1,0x4f,0x8e,0x9c,0xf1,
0x41,0xa0,0xd8,0x36,0x44,0x41,0xbc,0x26,0x0d,0xb3,0x40,0x32,0x9f,0x9b,0xb6,0x41,
0x32,0x93,0x1c,0xf5,0x3e,0xb1,0x0f,0xa8,0xf6,0x40,0xd0,0xf4,0x5c,0xd9,0xdb,0x0b,
0x4d,0x08,0xa5,0xe3,0x6c,0x77,0xaa,0x74,0xcc,0xd9,0x8d,0x55,0x74,0xf0,0x09,0x1e,
0x3f,0xd9,0x87,0x65,0x7d,0xfc,0xb8,0x62,0xfb,0xc9,0x87,0xfc,0xec,0x7d,0x6b,0xc0,
0xc0,0xec,0x71,0x87,0x05,0x26,0x28,0x94,0x49,0x10,0xee,0xf3,0x93,0x81,0x59,0x6b,
0x59,0xda,0xb2,0x7e,0x19,0xe1,0x0e,0xd7,0x89,0x5d,0xc3,0xb5,0x47,0x5c,0xd6,0xce,
0xd8,0xc6,0xe6,0xbc,0x29,0x3e,0x03,0x58,0x56,0x03,0x05,0x2a,0xc3,0xf8,0x6f,0x7c,
0x1b,0x57,0x9d,0x7a,0x89,0x4b,0xd7,0x5a,0x26,0xe0,0xb8,0x82,0xc8,0xac,0x2c,0x55,
0x1b,0x48,0x26,0x28,0x43,0x0c,0x82,0xb4,0x4e,0x33,0x09,0x61,0x5b,0x0f,0xc0,0xf4,
0xa9,0xdb,0x15,0xe4,0x5e,0x04,0x74,0x08,0x7d,0x2a,0x93,0x1e,0x06,0xa0,0x6f,0x7c,
0x98,0x71,0x4b,0xd3,0x7a,0x40,0x9b,0xce,0x1d,0x88,0x37,0xff,0xe9,0xdc,0xbd,0x32,
0xd3,0xb9,0x13,0x99,0xc6,0x2d,0x66,0x5b,0xd3,0xf7,0x04,0xc2,0x38,0xcd,0x64,0x45,
0x20,0x75,0x68,0x9b,0xbe,0x18,0xc7,0xae,0x84,0x00,0x6b,0x2c,0x2b,0x3a,0xe0,0x04,
0xff,0x78,0x57,0x10,0x4d,0xbc,0xa5,0x7c,0x07,0x51,0x06,0xbc,0x90,0x63,0xbc,0x81,
0xbc,0x01,0x4e,0xbf,0x79,0xcc,0xc0,0xe2,0x27,0xc2,0x81,0x18,0xcb,0x60,0x67,0x91,
0x00,0xae,0x06,0xc2,0xb6,0xd1,0x09,0x64,0x52,0x1c,0x13,0xce,0x8a,0xd8,0x36,0x04,
0xea,0x15,0x24,0x68,0x0d,0xb8,0xe1,0x3e,0xd8,0xd6,0x80,0xd5,0xf5,0xeb,0x2d,0xe8,
0x33,0xc8,0x0f,0x33,0x10,0x35,0x50,0x2c,0xd1,0x57,0x09,0x79,0x58,0x59,0x0a,0xa8,
0x10,0x14,0xe1,0x66,0x45,0x44,0xe8,0x53,0x86,0xe4,0x26,0x90,0x65,0xf4,0x40,0x3d,
0xcf,0x32,0xd1,0x12,0xde,0x50,0x04,0x46,0xba,0x01,0x54,0x45,0xa0,0x43,0x02,0xdf,
0xf8,0x9b,0x34,0x38,0x71,0x32,0x3c,0xc2,0xcf,0x8a,0xee,0xa5,0xac,0x84,0xb8,0xc1,
0x31,0x12,0x50,0x8d,0x40,0xe0,0x70,0x1a,0x3c,0x4a,0x47,0x33,0xe8,0x42,0x48,0xf0,
0x56,0x54,0x07,0x38,0x21,0x71,0x20,0xa0,0x7c,0x65,0x41,0xa2,0xb1,0xc4,0xeb,0x78,
0x70,0x20,0x41,0x63,0x4b,0x8f,0x14,0x24,0x10,0x19,0x4d,0xa6,0x07,0x15,0x6b,0x48,
0x83,0x84,0xd3,0x39,0x30,0x5e,0x27,0x5b,0xb8,0xfb,0x0a,0x11,0xd4,0x78,0x90,0x27,
0x92,0x8f,0x8b,0xcb,0xdb,0x9e,0xa6,0x30,0x40,0xc4,0xc9,0x4d,0xc6,0x07,0xf2,0x79,
0x83,0x20,0xcd,0x48,0x90,0x7a,0x58,0x86,0x8f,0x08,0xc1,0x56,0x88,0x2e,0xd3,0x30,
0x30,0xbe,0x29,0x80,0xfb,0xd0,0xc7,0x21,0x81,0x13,0xcc,0x45,0xed,0x8f,0x6a,0x43,
0xda,0x14,0x86,0x12,0xb9,0x46,0x92,0x89,0x12,0x21,0x6f,0x50,0x93,0xda,0x78,0x65,
0xff,0x3d,0x37,0xc6,0x8f,0x97,0x08,0x89,0xdd,0xab,0x06,0xd2,0x4a,0xcd,0xc8,0x32,
0x1a,0x0b,0x6b,0x40,0x1c,0x15,0x52,0x0f,0xd0,0xa5,0x47,0x02,0x84,0x43,0xe8,0x71,
0xfe,0xa3,0x9c,0x65,0x94,0x6a,0x40,0x51,0xfb,0x84,0x32,0x3e,0x01,0x8d,0x64,0x80,
0xe2,0x19,0xcc,0x61,0x4d,0x26,0x4c,0x91,0x0c,0x53,0xbc,0xc1,0x4c,0x68,0x99,0xe4,
0xa4,0xc2,0x43,0x4f,0xb6,0xf1,0x0d,0x21,0x43,0x1b,0xcb,0x8b,0x72,0x38,0x96,0xd9,
0x01,0x2b,0x69,0x33,0x0c,0x5e,0xd4,0x32,0xf1,0x2f,0x7a,0x1e,0x44,0x1f,0x9c,0xa9,
0xe6,0x41,0x64,0x64,0xa5,0x0c,0x7e,0x11,0x85,0x77,0x1a,0xa1,0x6c,0x76,0x56,0xbb,
0xfd,0xec,0xa7,0x43,0x6f,0x51,0xa1,0x78,0x38,0x28,0xbd,0xb1,0xf4,0x80,0x9b,0x7a,
0x22,0x97,0x35,0x71,0x2a,0x90,0x7a,0xd0,0x6e,0x61,0x00,0x25,0xdd,0x5b,0x56,0x64,
0x8a,0xb7,0x08,0x75,0x5b,0x70,0x74,0x88,0x1e,0x51,0xb5,0x18,0xf9,0x04,0x4d,0x32,
0xbf,0x69,0x0f,0x3d,0x24,0xb3,0x99,0xa8,0x91,0x0d,0x69,0x73,0x22,0xdb,0xde,0xb8,
0xc4,0x9b,0x79,0xae,0xc8,0x8e,0xef,0x53,0x48,0xdd,0x9c,0xb7,0x8f,0x1c,0xd2,0xc5,
0x8d,0xdc,0xbb,0xcb,0x59,0x7f,0x58,0x9e,0x81,0x78,0xb3,0x89,0x09,0xa9,0x95,0xf5,
0x64,0x59,0x90,0xb5,0x84,0x11,0xb2,0xfb,0x20,0x82,0x65,0xf3,0x12,0x9a,0x81,0x45,
0xa3,0x58,0xc9,0xc1,0xe5,0x7f,0x6c,0x34,0x2a,0x52,0xfd,0x87,0x6f,0x2e,0xd4,0x6b,
0x55,0x31,0x2b,0xc7,0x34,0x39,0xee,0x87,0xde,0x73,0xa0,0x5b,0x16,0x4a,0x10,0xb6,
0xff,0x51,0x60,0x41,0x31,0x9d,0x40,0x3b,0xbf,0xb8,0x26,0x70,0x26,0x11,0x7a,0x95,
0x51,0x4e,0xee,0xea,0x13,0x1c,0xb9,0xa2,0xca,0xa8,0xfd,0xd2,0x5b,0x70,0xac,0x07,
0xb8,0xaa,0x48,0x36,0x5a,0xbd,0x5c,0x69,0xbc,0x5a,0xba,0x26,0x0d,0x44,0xcd,0xab,
0x7c,0xa5,0xcb,0xed,0x4e,0x5a,0xd3,0x5f,0x81,0x55,0x39,0x6a,0xab,0x97,0x26,0xdc,
0x32,0x3b,0x25,0xee,0x71,0x96,0x02,0x01,0x61,0x58,0x17,0x42,0x1f,0xc4,0x49,0xa0,
0x68,0xb9,0x44,0x95,0x14,0x7b,0x09,0xd6,0xea,0xbc,0x68,0x4f,0xc3,0x59,0x0c,0x03,
0xfa,0xb6,0x29,0xd4,0x72,0xe6,0x7f,0x65,0x4b,0x57,0x93,0xc6,0x92,0x27,0x83,0x9c,
0x94,0x02,0x0c,0x78,0x69,0x3d,0xf1,0x05,0xa6,0x7f,0xfa,0x96,0x76,0x96,0x29,0x54,
0xcf,0x48,0x66,0x90,0x05,0x1e,0xe7,0xc1,0x05,0x99,0x0e,0x82,0x1c,0x10,0x56,0x63,
0x22,0xa7,0x01,0x4d,0x7b,0x28,0x6b,0x05,0x77,0x3f,0xdd,0xc6,0xac,0xa7,0x8d,0xe1,
0x5b,0xad,0x9a,0xf5,0x9f,0x31,0x4a,0x2a,0x6a,0xca,0x48,0xad,0x05,0xeb,0x90,0x19,
0x37,0xca,0x6c,0xb2,0x11,0xb4,0x6a,0xa1,0xf6,0x75,0x1c,0x38,0x40,0xad,0x30,0xb9,
0x51,0xec,0x43,0xc4,0x82,0xc8,0x8e,0x15,0x6d,0x52,0x28,0x92,0x8c,0x72,0xea,0xd1,
0x03,0x94,0xa2,0x13,0x87,0x48,0x26,0x82,0x1b,0x51,0x06,0xa5,0x5f,0xd5,0xa1,0x74,
0x50,0x72,0x31,0x07,0x6d,0x46,0x04,0x1f,0xfc,0xa6,0x01,0x66,0x4c,0xdd,0xf5,0x1c,
0xb8,0xbc,0x57,0xfe,0xf0,0xb6,0xb0,0x8b,0x8d,0x16,0xf1,0x85,0x96,0x21,0x60,0x90,
0xff,0xb6,0x08,0xa1,0xe5,0x74,0x6f,0x29,0x90,0x85,0x39,0x09,0x94,0x89,0x84,0xc6,
0x65,0xad,0x6a,0xc1,0x68,0x04,0x13,0xbf,0xff,0x99,0xc0,0x41,0xd6,0x94,0x19,0xf9,
0xd8,0x71,0x02,0xe5,0xd1,0x6c,0x9b,0xbe,0x6c,0x62,0x83,0xd0,0xd4,0xa7,0x3f,0xa4,
0x33,0x76,0x09,0x62,0x44,0xb0,0x70,0x93,0x58,0x6a,0xd2,0x80,0x9b,0x18,0x90,0xb6,
0x9b,0x22,0x77,0x21,0xf1,0xd3,0x80,0x6e,0xca,0x69,0x30,0x80,0x01,0x34,0x1a,0x5e,
0x7e,0x4c,0xf1,0x28,0xfb,0x2e,0xba,0xee,0xa8,0x2c,0x05,0x26,0x48,0xcf,0x30,0x64,
0xca,0x52,0xd4,0x46,0x49,0x2b,0x7e,0xcb,0xdf,0x12,0xa2,0x89,0x43,0xe2,0xcf,0x6a,
0x5e,0x9e,0x00,0x56,0x69,0xe7,0x98,0xc6,0x06,0xd4,0x0d,0x1a,0x38,0x24,0x11,0xda,
0x93,0x0c,0xc6,0xe4,0xeb,0x21,0xfb,0xa9,0x4e,0x11,0xa0,0xa1,0x09,0x53,0x5c,0xac,
0x4c,0x9c,0x79,0x03,0x73,0x82,0x57,0x9b,0xb7,0x24,0x2b,0x13,0x4a,0x52,0x0d,0x3d,
0x9c,0x96,0x4f,0x54,0x91,0x89,0x64,0x62,0xf3,0x73,0x3d,0xba,0x45,0x17,0xf9,0xf0,
0x6a,0x32,0x30,0x4e,0x94,0x1b,0x6b,0x62,0x87,0xb3,0xec,0xd6,0x20,0x86,0x1b,0x67,
0x70,0x92,0x76,0xa5,0x28,0xba,0xe5,0x2c,0xa2,0xf6,0x4e,0x70,0x28,0x50,0x14,0xc1,
0xdd,0xd5,0x36,0x21,0xbb,0x5c,0xdd,0x9c,0xea,0x62,0x20,0xca,0x24,0x51,0xca,0xb0,
0xe0,0x05,0xe5,0x25,0xf1,0x38,0x43,0xe9,0x49,0xf1,0xca,0xd2,0x93,0x26,0xca,0x1f,
0xa7,0x8e,0x09,0x3f,0x51,0xda,0x38,0x5c,0xd9,0x03,0x4d,0x99,0xca,0xc4,0x38,0x3d,
0xff,0xe8,0xdc,0x41,0x74,0x47,0x81,0x87,0xcc,0xd5,0x7e,0xe3,0xc1,0x0d,0x86,0x04,
0x1e,0xf2,0x11,0x41,0x09,0x47,0xbd,0x89,0x53,0xb7,0x37,0x33,0x4f,0x23,0x95,0xdc,
0xa7,0x2e,0x29,0x77,0xa3,0x2b,0xcc,0x19,0x95,0xb3,0x17,0x2d,0x53,0x1a,0x38,0x71,
0x19,0xc3,0xa9,0x73,0xde,0xe6,0x46,0xf9,0x41,0x53,0x8e,0x54,0x7d,0x73,0xe1,0x34,
0xe6,0xaf,0x4b,0x1c,0x76,0x4a,0x6a,0x47,0x04,0xea,0x45,0xc3,0xc2,0x26,0x7f,0x08,
0x7e,0x05,0x04,0x73,0x73,0xef,0x3b,0x36,0x00,0x46,0x5a,0x99,0x4e,0x65,0xf5,0x6e,
0xb7,0x5d,0xea,0x18,0x1d,0x48,0x29,0x7c,0xa6,0xf5,0x94,0x4c,0x98,0x31,0x45,0x70,
0x83,0x26,0x4e,0xb6,0x29,0x0c,0x23,0x04,0xc1,0x0e,0xfd,0x97,0x5c,0x90,0x05,0xba,
0xb6,0x20,0xeb,0x7e,0x87,0x0f,0x4b,0x9f,0x50,0xe9,0x50,0x78,0xb5,0xa5,0x66,0x6c,
0xf6,0xca,0xe2,0xc7,0x02,0x2f,0x64,0xd9,0x8a,0xcd,0xb3,0x4b,0x1d,0x88,0x67,0x92,
0x3b,0x5c,0x8d,0x29,0x4d,0x6f,0x79,0x19,0x45,0xae,0xa2,0x93,0x47,0x16,0xe7,0x91,
0x3a,0x19,0x55,0x11,0xf2,0xae,0x93,0x46,0x16,0x47,0xf5,0xa8,0xc7,0x14,0x11,0x1e,
0xa9,0x04,0x46,0xce,0x9e,0x08,0xaf,0xc7,0x7d,0xde,0x95,0x80,0x7b,0xd3,0xdf,0x5e,
0x27,0x69,0x28,0x02,0xef,0x51,0x0f,0x0a,0x0b,0xce,0xbd,0xbb,0x34,0xe1,0xde,0x5f,
0xf4,0xa6,0xe0,0x30,0x6d,0xa4,0x1e,0xd1,0xd0,0x47,0xc5,0x63,0xa2,0x8c,0x37,0xd0,
0xe1,0x0d,0x6e,0x78,0xc3,0x0c,0x9d,0xcf,0xfd,0xee,0x7b,0xff,0xfb,0xe0,0x0f,0xbf,
0x15,0xf8,0xc7,0x4f,0xfe,0xf2,0x9b,0xff,0xfc,0xe8,0x4f,0xbf,0xfa,0xd7,0xcf,0xfe,
0xf6,0xbb,0xdf,0x22,0x01,0x01,0x00,0x3b,};

static const unsigned int dummy_align__404_html = 1;
static const unsigned char data__404_html[] = {
/* /404.html (10 chars) */
0x2f,0x34,0x30,0x34,0x2e,0x68,0x74,0x6d,0x6c,0x00,0x00,0x00,

/* HTTP header */
/* "HTTP/1.1 404 File not found
" (29 bytes) */
0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x31,0x20,0x34,0x30,0x34,0x20,0x46,0x69,0x6c,
0x65,0x20,0x6e,0x6f,0x74,0x20,0x66,0x6f,0x75,0x6e,0x64,0x0d,0x0a,
/* "Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
" (63 bytes) */
0x53,0x65,0x72,0x76,0x65,0x72,0x3a,0x20,0x6c,0x77,0x49,0x50,0x2f,0x31,0x2e,0x33,
0x2e,0x31,0x20,0x28,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x73,0x61,0x76,0x61,0x6e,
0x6e,0x61,0x68,0x2e,0x6e,0x6f,0x6e,0x67,0x6e,0x75,0x2e,0x6f,0x72,0x67,0x2f,0x70,
0x72,0x6f,0x6a,0x65,0x63,0x74,0x73,0x2f,0x6c,0x77,0x69,0x70,0x29,0x0d,0x0a,
/* "Content-Length: 568
" (18+ bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x4c,0x65,0x6e,0x67,0x74,0x68,0x3a,0x20,
0x35,0x36,0x38,0x0d,0x0a,
/* "Connection: Close
" (19 bytes) */
0x43,0x6f,0x6e,0x6e,0x65,0x63,0x74,0x69,0x6f,0x6e,0x3a,0x20,0x43,0x6c,0x6f,0x73,
0x65,0x0d,0x0a,
/* "Content-type: text/html

" (27 bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x74,0x79,0x70,0x65,0x3a,0x20,0x74,0x65,
0x78,0x74,0x2f,0x68,0x74,0x6d,0x6c,0x0d,0x0a,0x0d,0x0a,
/* raw file data (568 bytes) */
0x3c,0x68,0x74,0x6d,0x6c,0x3e,0x0d,0x0a,0x3c,0x68,0x65,0x61,0x64,0x3e,0x3c,0x74,
0x69,0x74,0x6c,0x65,0x3e,0x4e,0x65,0x74,0x50,0x69,0x6e,0x67,0x20,0x70,0x61,0x67,
0x65,0x20,0x65,0x72,0x72,0x6f,0x72,0x3c,0x2f,0x74,0x69,0x74,0x6c,0x65,0x3e,0x3c,
0x2f,0x68,0x65,0x61,0x64,0x3e,0x0d,0x0a,0x3c,0x62,0x6f,0x64,0x79,0x20,0x62,0x67,
0x63,0x6f,0x6c,0x6f,0x72,0x3d,0x22,0x77,0x68,0x69,0x74,0x65,0x22,0x20,0x74,0x65,
0x78,0x74,0x3d,0x22,0x62,0x6c,0x61,0x63,0x6b,0x22,0x3e,0x0d,0x0a,0x0d,0x0a,0x20,
0x20,0x20,0x20,0x3c,0x74,0x61,0x62,0x6c,0x65,0x20,0x77,0x69,0x64,0x74,0x68,0x3d,
0x22,0x31,0x30,0x30,0x25,0x22,0x3e,0x0d,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,
0x74,0x72,0x20,0x76,0x61,0x6c,0x69,0x67,0x6e,0x3d,0x22,0x74,0x6f,0x70,0x22,0x3e,
0x3c,0x74,0x64,0x20,0x77,0x69,0x64,0x74,0x68,0x3d,0x22,0x38,0x30,0x22,0x3e,0x09,
0x20,0x20,0x0d,0x0a,0x09,0x20,0x20,0x3c,0x61,0x20,0x68,0x72,0x65,0x66,0x3d,0x22,
0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x77,0x77,0x77,0x2e,0x6e,0x65,0x74,0x70,0x69,
0x6e,0x67,0x2e,0x72,0x75,0x2f,0x22,0x3e,0x3c,0x69,0x6d,0x67,0x20,0x73,0x72,0x63,
0x3d,0x22,0x69,0x6d,0x67,0x2f,0x6e,0x65,0x74,0x70,0x69,0x6e,0x67,0x2e,0x67,0x69,
0x66,0x22,0x20,0x68,0x65,0x69,0x67,0x68,0x74,0x3d,0x22,0x35,0x39,0x22,0x20,0x77,
0x69,0x64,0x74,0x68,0x3d,0x22,0x31,0x36,0x39,0x22,0x0d,0x0a,0x09,0x20,0x20,0x62,
0x6f,0x72,0x64,0x65,0x72,0x3d,0x22,0x30,0x22,0x20,0x61,0x6c,0x74,0x3d,0x22,0x6e,
0x65,0x74,0x70,0x69,0x6e,0x67,0x20,0x6c,0x6f,0x67,0x6f,0x22,0x20,0x74,0x69,0x74,
0x6c,0x65,0x3d,0x22,0x34,0x30,0x34,0x20,0x65,0x72,0x72,0x6f,0x72,0x22,0x3e,0x3c,
0x2f,0x61,0x3e,0x0d,0x0a,0x09,0x3c,0x2f,0x74,0x64,0x3e,0x3c,0x74,0x64,0x20,0x77,
0x69,0x64,0x74,0x68,0x3d,0x22,0x35,0x30,0x30,0x22,0x3e,0x09,0x20,0x20,0x0d,0x0a,
0x09,0x20,0x20,0x3c,0x68,0x31,0x3e,0x4e,0x65,0x74,0x50,0x69,0x6e,0x67,0x20,0x70,
0x61,0x67,0x65,0x20,0x65,0x72,0x72,0x6f,0x72,0x21,0x3c,0x2f,0x68,0x31,0x3e,0x0d,
0x0a,0x09,0x20,0x20,0x3c,0x68,0x32,0x3e,0x34,0x30,0x34,0x20,0x2d,0x20,0x50,0x61,
0x67,0x65,0x20,0x6e,0x6f,0x74,0x20,0x66,0x6f,0x75,0x6e,0x64,0x3c,0x2f,0x68,0x32,
0x3e,0x0d,0x0a,0x09,0x20,0x20,0x3c,0x70,0x3e,0x0d,0x0a,0x09,0x20,0x20,0x20,0x20,
0x53,0x6f,0x72,0x72,0x79,0x2c,0x20,0x74,0x68,0x65,0x20,0x70,0x61,0x67,0x65,0x20,
0x79,0x6f,0x75,0x20,0x61,0x72,0x65,0x20,0x72,0x65,0x71,0x75,0x65,0x73,0x74,0x69,
0x6e,0x67,0x20,0x77,0x61,0x73,0x20,0x6e,0x6f,0x74,0x20,0x66,0x6f,0x75,0x6e,0x64,
0x20,0x6f,0x6e,0x20,0x74,0x68,0x69,0x73,0x0d,0x0a,0x09,0x20,0x20,0x20,0x20,0x73,
0x65,0x72,0x76,0x65,0x72,0x2e,0x20,0x0d,0x0a,0x09,0x20,0x20,0x3c,0x2f,0x70,0x3e,
0x0d,0x0a,0x09,0x3c,0x2f,0x74,0x64,0x3e,0x3c,0x74,0x64,0x3e,0x0d,0x0a,0x09,0x20,
0x20,0x26,0x6e,0x62,0x73,0x70,0x3b,0x0d,0x0a,0x09,0x3c,0x2f,0x74,0x64,0x3e,0x3c,
0x2f,0x74,0x72,0x3e,0x0d,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x2f,0x74,0x61,
0x62,0x6c,0x65,0x3e,0x0d,0x0a,0x3c,0x2f,0x62,0x6f,0x64,0x79,0x3e,0x0d,0x0a,0x3c,
0x2f,0x68,0x74,0x6d,0x6c,0x3e,0x0d,0x0a,};



const struct fsdata_file file__img_netping_gif[] = { {
file_NULL,
data__img_netping_gif,
data__img_netping_gif + 20,
sizeof(data__img_netping_gif) - 20,
1,
}};

const struct fsdata_file file__404_html[] = { {
file__img_netping_gif,
data__404_html,
data__404_html + 12,
sizeof(data__404_html) - 12,
1,
}};

#define FS_ROOT file__404_html
#define FS_NUMFILES 2
