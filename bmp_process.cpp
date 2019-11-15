#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


#pragma pack(1)


typedef unsigned char  BYTE;//1����Ʈ
typedef unsigned short WORD;//2����Ʈ
typedef unsigned long  DWORD;//4����Ʈ
typedef long           LONG;//4����Ʈ


typedef struct _BITMAPFILEHEADER {
	WORD    bfType;//BMP ���� ���� �ѹ�. ��Ʈ�� ������ �´��� Ȯ���ϴµ� ����ϸ� ASCII �ڵ�� 0x42(B), 0x4D(M)�� ����˴ϴ�.
	DWORD   bfSize;//���� ũ��(����Ʈ)
	WORD    bfReserved1;//����� ������� ������ �̷��� ���� ����� ����
	WORD    bfReserved2;//����� ������� ������ �̷��� ���� ����� ����
	DWORD   bfOffBits;//��Ʈ�� �������� ���� ��ġ
} BITMAPFILEHEADER;



typedef struct _BITMAPINFOHEADER {
	DWORD  biSize;//���� ��Ʈ�� ���� ���(BITMAPINFOHEADER)�� ũ��
	LONG   biWidth;//��Ʈ�� �̹����� ���� ũ��(�ȼ�)
	LONG   biHeight;//��Ʈ�� �̹����� ���� ũ��(�ȼ�). #���: �̹����� ���ϰ� �������� ����� ���� #����: �̹����� �״�� ����� ���� ..���� ���� ũ��� ����� ����Ǿ� �ֽ��ϴ�.
	WORD   biPlanes;//����ϴ� �������� ��. �׻� 1�Դϴ�.
	WORD   biBitCount;//�ȼ� �ϳ��� ǥ���ϴ� ��Ʈ ��
	DWORD  biCompression;//���� ���. ���� ��Ʈ���� ������ ���� �����Ƿ� 0�Դϴ�.
	DWORD  biSizeImage;//��Ʈ�� �̹����� �ȼ� ������ ũ��(���� ���� ���� ũ��)
	LONG   biXPelsPerMeter;//�׸��� ���� �ػ�(���ʹ� �ȼ�)
	LONG   biYPelsPerMeter;//�׸��� ���� �ػ�(���ʹ� �ȼ�)
	DWORD  biClrUsed;//���� ���̺��� ���� ���Ǵ� ���� ��
	DWORD  biClrImportant;//��Ʈ���� ǥ���ϱ� ���� �ʿ��� ���� �ε��� ��
} BITMAPINFOHEADER;



//24��Ʈ ��Ʈ���� �ȼ�(RGBTRIPLE)�� �Ķ�(B),�ʷ�(G), ����(R) ������ �����ϸ� �� ������ ũ��� 1����Ʈ�Դϴ�. ���� �ȼ��� 3����Ʈ�� ����մϴ�.
typedef struct _RGBTRIPLE {
	BYTE   rgbtBlue;//1����Ʈ
	BYTE   rgbtGreen;//1����Ʈ
	BYTE   rgbtRed;//1����Ʈ
} RGBTRIPLE;



void make_gray(RGBTRIPLE* pix, BITMAPINFOHEADER* bmih);

void make_reverse(RGBTRIPLE* pix, BITMAPINFOHEADER* bmih);

void make_thicker(RGBTRIPLE* pix, BITMAPINFOHEADER* bmih);

void make_thiner(RGBTRIPLE* pix, BITMAPINFOHEADER* bmih);



int main()
{
	BITMAPFILEHEADER bmfh;//��Ʈ�� ���� ���
	BITMAPINFOHEADER bmih;//��Ʈ�� ���� ���
	RGBTRIPLE* pix;//24��Ʈ ��Ʈ���� RGB �ȼ� ��
	int a, x;
	FILE* fp;
	FILE* ck;
	FILE* wt;


	//-------------------------���� ����-------------------------//
	if ((fp = fopen("Lenna.bmp", "rb")) == NULL)
	{
		printf("file is not opened qt\n");
		return 1;
	}
	//printf("���� ����\n\n");
   //-------------------------�о���̱�-------------------------//
	/*fread(void* ptr, size_t size, size_t count, FILE* stream)  ptr�� stream������ size��ŭ �迭�� ������ count��ŭ ����*/
	fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp);  // ������� �о����
	fread(&bmih, sizeof(BITMAPINFOHEADER), 1, fp);  // ������� �о����
	pix = (RGBTRIPLE*)malloc(bmih.biSizeImage);     // �̹��� ���� �Ҵ�
	fread(pix, 1, bmih.biSizeImage, fp);            // �̹��� ������ �о����
	fclose(fp);
	//printf("\nread complete\n");
   //-------------------------���� �״�� txt ����-------------------------//
	fp = fopen("checking.txt", "w+");
	/*fwrite(const void* ptr, size_t size, size_t count, FILE* stream) stream�� ptr�� size��ŭ �迭������ count��ŭ ��*/
	fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp); // ������� ����
	for (x = 0; x < (bmih.biWidth * bmih.biHeight); x++)
		fprintf(fp, "[%d]: (%x)(%x)(%x)\t", x, pix[x].rgbtRed, pix[x].rgbtGreen, pix[x].rgbtBlue); //����� ���� �о�鿴�� ������ ��� ������ �ٽ� �о���̴� pix[]�� ��� �������� �Է��� �ϴµ�. �ᱹ pix[]�迭�� �迭 54���� ������ 55��°���� �ԷµǴ°�.
	fclose(fp);
	//printf("\nwt complete\n");
   //-------------------------���� �״�� �ٽ� bmp ����-------------------------//
   /* ck=fopen("checking.bmp", "wb");
	fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp); // ������� ����
	fwrite(&bmih, sizeof(BITMAPINFOHEADER), 1, fp); // ������� ����
	fwrite(pix, 1, bmih.biSizeImage, fp);           // �̹��� ������ ����
	free(pix);
	fclose(fp);
   */


   //���α׷� ȭ��
	printf("����ȯ�� ���� ���α׷��Դϴ�.\
		\n��ȯ�ϰ��� �ϴ� ��ȣ�� �Է��ϰ� ���͸� ��������.\
		\n1. ��麯ȯ.\n2. ����ȯ.\n3. ������ ��Ӱ�.\n4. ������ ���ϰ�.\n5. �״��.\n");
	scanf("%d", &a);

	switch (a)
	{
	case 1://��麯ȯ
	{
		make_gray(pix, &bmih);  //�迭�� �Լ��� �������ڷ� �ѱ�ÿ��� (*pix)�̰� �ƴ϶� �׳� (pix)�� ����Ѵ�
		wt = fopen("gray.bmp", "w+"); //�ƿ��� wb��� ���� �ȵǰ� w+�� ���� �Ǵ±���. �ֱ׷����� �� �𸣰ڴٸ�...
		fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, wt); // ������� ����
		fwrite(&bmih, sizeof(BITMAPINFOHEADER), 1, wt); // ������� ����
		fwrite(pix, 1, bmih.biSizeImage, wt);           // �̹��� ������ ����
		free(pix);
		fclose(wt);
		break;
	}

	case 2://����ȯ
	{
		make_reverse(pix, &bmih);
		wt = fopen("reverse.bmp", "wb");
		fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, wt); // ������� ����
		fwrite(&bmih, sizeof(BITMAPINFOHEADER), 1, wt); // ������� ����
		fwrite(pix, 1, bmih.biSizeImage, wt);           // �̹��� ������ ����
		free(pix);
		fclose(wt);
		break;
	}

	case 3://��Ӱ� ��ȯ
	{
		make_thicker(pix, &bmih);
		wt = fopen("thicker.bmp", "wb");
		fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, wt); // ������� ����
		fwrite(&bmih, sizeof(BITMAPINFOHEADER), 1, wt); // ������� ����
		fwrite(pix, 1, bmih.biSizeImage, wt);           // �̹��� ������ ����
		free(pix);
		fclose(wt);
		break;
	}

	case 4://��� ��ȯ
	{
		make_thiner(pix, &bmih);
		wt = fopen("thiner.bmp", "wb");
		fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, wt); // ������� ����
		fwrite(&bmih, sizeof(BITMAPINFOHEADER), 1, wt); // ������� ����
		fwrite(pix, 1, bmih.biSizeImage, wt);           // �̹��� ������ ����
		free(pix);
		fclose(wt);
		break;
	}

	case 5://�׳����
	{
		wt = fopen("non.bmp", "wb");
		fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, wt); // ������� ����
		fwrite(&bmih, sizeof(BITMAPINFOHEADER), 1, wt); // ������� ����
		fwrite(pix, 1, bmih.biSizeImage, wt);           // �̹��� ������ ����
		fclose(wt);
		free(pix);
		break;
	}

	}

	return 0;

}





void make_gray(RGBTRIPLE* pix, BITMAPINFOHEADER* bmih)//��麯ȯ �Լ�
{
	int sizeofarr = (bmih->biWidth * bmih->biHeight);
	int newpix, x;  //��ȯ�� ���� ����ϴ� ����
	for (x = 0; x < sizeofarr - 1; x++)
		{
		 newpix = (pix[x].rgbtBlue + pix[x].rgbtRed + pix[x].rgbtGreen) / 3;
		 pix[x].rgbtBlue = pix[x].rgbtRed = pix[x].rgbtGreen = newpix;
		}
}


void make_reverse(RGBTRIPLE* pix, BITMAPINFOHEADER* bmih) //������ ������ ��´�
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


void make_thiner(RGBTRIPLE* pix, BITMAPINFOHEADER* bmih)//��� �ϴ� �Լ�
{
	int sizeofarr = (bmih->biWidth * bmih->biHeight);
	int x, v;
	for (x = 0; x < sizeofarr - 1; x++)
		{
		 v = (int)pix[x].rgbtRed + 100; //���� ����ü�� ���� rgbtRed, rgbtBlue, rgbtGreen�� int���� �ƴ϶� unsigned���̴�. �׷��� ���� ��ȣ�� ��������. �� ���� �������� �ʰ� �ڵ��ؼ� ����.
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


void make_thicker(RGBTRIPLE* pix, BITMAPINFOHEADER* bmih)//��Ӱ� �ϴ� �Լ�
{
	int sizeofarr = (bmih->biWidth * bmih->biHeight);
	int x, v;
	/*printf("\n%d\n", pix[100].rgbtRed);*/

	for (x = 0; x < sizeofarr - 1; x++)
		{
		 v = (int)pix[x].rgbtRed - 100; //���� ����ü�� ���� rgbtRed, rgbtBlue, rgbtGreen�� int���� �ƴ϶� unsigned���̴�. �׷��� ���� ��ȣ�� ��������. �� ���� �������� �ʰ� �ڵ��ؼ� ����.
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