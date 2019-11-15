#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


#pragma pack(1)


typedef unsigned char  BYTE;//1바이트
typedef unsigned short WORD;//2바이트
typedef unsigned long  DWORD;//4바이트
typedef long           LONG;//4바이트


typedef struct _BITMAPFILEHEADER {
	WORD    bfType;//BMP 파일 매직 넘버. 비트맵 파일이 맞는지 확인하는데 사용하며 ASCII 코드로 0x42(B), 0x4D(M)가 저장됩니다.
	DWORD   bfSize;//파일 크기(바이트)
	WORD    bfReserved1;//현재는 사용하지 않으며 미래를 위해 예약된 공간
	WORD    bfReserved2;//현재는 사용하지 않으며 미래를 위해 예약된 공간
	DWORD   bfOffBits;//비트맵 데이터의 시작 위치
} BITMAPFILEHEADER;



typedef struct _BITMAPINFOHEADER {
	DWORD  biSize;//현재 비트맵 정보 헤더(BITMAPINFOHEADER)의 크기
	LONG   biWidth;//비트맵 이미지의 가로 크기(픽셀)
	LONG   biHeight;//비트맵 이미지의 세로 크기(픽셀). #양수: 이미지의 상하가 뒤집혀서 저장된 상태 #음수: 이미지가 그대로 저장된 상태 ..보통 세로 크기는 양수로 저장되어 있습니다.
	WORD   biPlanes;//사용하는 색상판의 수. 항상 1입니다.
	WORD   biBitCount;//픽셀 하나를 표현하는 비트 수
	DWORD  biCompression;//압축 방식. 보통 비트맵은 압축을 하지 않으므로 0입니다.
	DWORD  biSizeImage;//비트맵 이미지의 픽셀 데이터 크기(압축 되지 않은 크기)
	LONG   biXPelsPerMeter;//그림의 가로 해상도(미터당 픽셀)
	LONG   biYPelsPerMeter;//그림의 세로 해상도(미터당 픽셀)
	DWORD  biClrUsed;//색상 테이블에서 실제 사용되는 색상 수
	DWORD  biClrImportant;//비트맵을 표현하기 위해 필요한 색상 인덱스 수
} BITMAPINFOHEADER;



//24비트 비트맵은 픽셀(RGBTRIPLE)을 파랑(B),초록(G), 빨강(R) 순서로 저장하며 각 색상의 크기는 1바이트입니다. 따라서 픽셀당 3바이트를 사용합니다.
typedef struct _RGBTRIPLE {
	BYTE   rgbtBlue;//1바이트
	BYTE   rgbtGreen;//1바이트
	BYTE   rgbtRed;//1바이트
} RGBTRIPLE;



void make_gray(RGBTRIPLE* pix, BITMAPINFOHEADER* bmih);

void make_reverse(RGBTRIPLE* pix, BITMAPINFOHEADER* bmih);

void make_thicker(RGBTRIPLE* pix, BITMAPINFOHEADER* bmih);

void make_thiner(RGBTRIPLE* pix, BITMAPINFOHEADER* bmih);



int main()
{
	BITMAPFILEHEADER bmfh;//비트맵 파일 헤더
	BITMAPINFOHEADER bmih;//비트맵 정보 헤더
	RGBTRIPLE* pix;//24비트 비트맵의 RGB 픽셀 값
	int a, x;
	FILE* fp;
	FILE* ck;
	FILE* wt;


	//-------------------------파일 열기-------------------------//
	if ((fp = fopen("Lenna.bmp", "rb")) == NULL)
	{
		printf("file is not opened qt\n");
		return 1;
	}
	//printf("파일 열림\n\n");
   //-------------------------읽어들이기-------------------------//
	/*fread(void* ptr, size_t size, size_t count, FILE* stream)  ptr에 stream파일을 size만큼 배열의 갯수인 count만큼 저장*/
	fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp);  // 파일헤더 읽어들임
	fread(&bmih, sizeof(BITMAPINFOHEADER), 1, fp);  // 정보헤더 읽어들임
	pix = (RGBTRIPLE*)malloc(bmih.biSizeImage);     // 이미지 공간 할당
	fread(pix, 1, bmih.biSizeImage, fp);            // 이미지 데이터 읽어들임
	fclose(fp);
	//printf("\nread complete\n");
   //-------------------------읽은 그대로 txt 쓰기-------------------------//
	fp = fopen("checking.txt", "w+");
	/*fwrite(const void* ptr, size_t size, size_t count, FILE* stream) stream에 ptr을 size만큼 배열갯수인 count만큼 씀*/
	fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp); // 파일헤더 저장
	for (x = 0; x < (bmih.biWidth * bmih.biHeight); x++)
		fprintf(fp, "[%d]: (%x)(%x)(%x)\t", x, pix[x].rgbtRed, pix[x].rgbtGreen, pix[x].rgbtBlue); //헤더만 먼저 읽어들였기 때문에 헤더 다음에 다시 읽어들이는 pix[]는 헤더 다음부터 입력을 하는듯. 결국 pix[]배열은 배열 54개를 제외한 55번째부터 입력되는것.
	fclose(fp);
	//printf("\nwt complete\n");
   //-------------------------읽은 그대로 다시 bmp 쓰기-------------------------//
   /* ck=fopen("checking.bmp", "wb");
	fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp); // 파일헤더 저장
	fwrite(&bmih, sizeof(BITMAPINFOHEADER), 1, fp); // 정보헤더 저장
	fwrite(pix, 1, bmih.biSizeImage, fp);           // 이미지 데이터 저장
	free(pix);
	fclose(fp);
   */


   //프로그램 화면
	printf("영상변환을 위한 프로그램입니다.\
		\n변환하고자 하는 번호를 입력하고 엔터를 누르세요.\
		\n1. 흑백변환.\n2. 역상변환.\n3. 사진을 어둡게.\n4. 사진을 연하게.\n5. 그대로.\n");
	scanf("%d", &a);

	switch (a)
	{
	case 1://흑백변환
	{
		make_gray(pix, &bmih);  //배열을 함수의 전달인자로 넘길시에는 (*pix)이게 아니라 그냥 (pix)로 써야한다
		wt = fopen("gray.bmp", "w+"); //아오씨 wb라고 쓰니 안되고 w+라 쓰니 되는구나. 왜그런지는 잘 모르겠다만...
		fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, wt); // 파일헤더 저장
		fwrite(&bmih, sizeof(BITMAPINFOHEADER), 1, wt); // 정보헤더 저장
		fwrite(pix, 1, bmih.biSizeImage, wt);           // 이미지 데이터 저장
		free(pix);
		fclose(wt);
		break;
	}

	case 2://역상변환
	{
		make_reverse(pix, &bmih);
		wt = fopen("reverse.bmp", "wb");
		fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, wt); // 파일헤더 저장
		fwrite(&bmih, sizeof(BITMAPINFOHEADER), 1, wt); // 정보헤더 저장
		fwrite(pix, 1, bmih.biSizeImage, wt);           // 이미지 데이터 저장
		free(pix);
		fclose(wt);
		break;
	}

	case 3://어둡게 변환
	{
		make_thicker(pix, &bmih);
		wt = fopen("thicker.bmp", "wb");
		fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, wt); // 파일헤더 저장
		fwrite(&bmih, sizeof(BITMAPINFOHEADER), 1, wt); // 정보헤더 저장
		fwrite(pix, 1, bmih.biSizeImage, wt);           // 이미지 데이터 저장
		free(pix);
		fclose(wt);
		break;
	}

	case 4://밝게 변환
	{
		make_thiner(pix, &bmih);
		wt = fopen("thiner.bmp", "wb");
		fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, wt); // 파일헤더 저장
		fwrite(&bmih, sizeof(BITMAPINFOHEADER), 1, wt); // 정보헤더 저장
		fwrite(pix, 1, bmih.biSizeImage, wt);           // 이미지 데이터 저장
		free(pix);
		fclose(wt);
		break;
	}

	case 5://그냥출력
	{
		wt = fopen("non.bmp", "wb");
		fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, wt); // 파일헤더 저장
		fwrite(&bmih, sizeof(BITMAPINFOHEADER), 1, wt); // 정보헤더 저장
		fwrite(pix, 1, bmih.biSizeImage, wt);           // 이미지 데이터 저장
		fclose(wt);
		free(pix);
		break;
	}

	}

	return 0;

}





void make_gray(RGBTRIPLE* pix, BITMAPINFOHEADER* bmih)//흑백변환 함수
{
	int sizeofarr = (bmih->biWidth * bmih->biHeight);
	int newpix, x;  //변환을 위해 사용하는 변수
	for (x = 0; x < sizeofarr - 1; x++)
		{
		 newpix = (pix[x].rgbtBlue + pix[x].rgbtRed + pix[x].rgbtGreen) / 3;
		 pix[x].rgbtBlue = pix[x].rgbtRed = pix[x].rgbtGreen = newpix;
		}
}


void make_reverse(RGBTRIPLE* pix, BITMAPINFOHEADER* bmih) //파일의 역상을 얻는다
{
	int sizeofarr = (bmih->biWidth * bmih->biHeight);
	int x;
	for (x = 0; x < sizeofarr - 1; x++)
		{
		 pix[x].rgbtRed = 255 - (pix[x].rgbtRed); //0xff;
		 pix[x].rgbtGreen = 255 - (pix[x].rgbtGreen); //0xff;
		 pix[x].rgbtBlue = 255 - (pix[x].rgbtBlue); //0xff;
		}
}


void make_thiner(RGBTRIPLE* pix, BITMAPINFOHEADER* bmih)//밝게 하는 함수
{
	int sizeofarr = (bmih->biWidth * bmih->biHeight);
	int x, v;
	for (x = 0; x < sizeofarr - 1; x++)
		{
		 v = (int)pix[x].rgbtRed + 100; //위의 구조체를 보면 rgbtRed, rgbtBlue, rgbtGreen은 int형이 아니라 unsigned형이다. 그래서 음의 부호가 없었던것. 이 점을 생각하지 않고 코딩해서 실패.
		 if (v >= 255) v = 255;
		 pix[x].rgbtRed = v;


		 v = (int)pix[x].rgbtGreen + 100;
		 if (v >= 255) v = 255;
		 pix[x].rgbtGreen = v;


		 v = (int)pix[x].rgbtBlue + 100;
		 if (v >= 255) v = 255;
		 pix[x].rgbtBlue = v;
		 /*  pix[x].rgbtRed=(pix[x].rgbtRed)+30;
		   if(pix[x].rgbtRed>255)
			pix[x].rgbtRed=255;

		   pix[x].rgbtGreen=(pix[x].rgbtGreen)+30;
		   if(pix[x].rgbtGreen>255)
			pix[x].rgbtGreen=255;

		   pix[x].rgbtBlue=(pix[x].rgbtBlue)+30;
		   if(pix[x].rgbtBlue>255)
			pix[x].rgbtBlue=255;
		 */
		}
}


void make_thicker(RGBTRIPLE* pix, BITMAPINFOHEADER* bmih)//어둡게 하는 함수
{
	int sizeofarr = (bmih->biWidth * bmih->biHeight);
	int x, v;
	/*printf("\n%d\n", pix[100].rgbtRed);*/

	for (x = 0; x < sizeofarr - 1; x++)
		{
		 v = (int)pix[x].rgbtRed - 100; //위의 구조체를 보면 rgbtRed, rgbtBlue, rgbtGreen은 int형이 아니라 unsigned형이다. 그래서 음의 부호가 없었던것. 이 점을 생각하지 않고 코딩해서 실패.
		 if (v <= 0) v = 0;
		 pix[x].rgbtRed = v;


		 v = (int)pix[x].rgbtGreen - 100;
		 if (v <= 0) v = 0;
		 pix[x].rgbtGreen = v;


		 v = (int)pix[x].rgbtBlue - 100;
		 if (v <= 0) v = 0;
		 pix[x].rgbtBlue = v;

		 //(int)pix[x].rgbtBlue=(int)pix[x].rgbtBlue-10;
		 //if(pix[x].rgbtBlue<=0)
		 // pix[x].rgbtBlue=0;
		}
	/*printf("\n%d\n", pix[100].rgbtRed);*/

}