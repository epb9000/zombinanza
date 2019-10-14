/*Graphics routine!*/
void DrawAlpha(HDC hdestdc, HDC hsrcdc, HDC alphamaskdc, int destx, int desty, int srcx, int srcy, int maskx, int masky, int width, int height)
{
     HBITMAP temp[3];
     HDC tempdc[4];
     HGDIOBJ old[4];
     LPBITMAPINFO bmpinfoheader[3]; /*bmpinfoheaders for the bitmaps*/
     BYTE FAR* bmpbits1;
     BYTE FAR* bmpbits2;
     BYTE FAR* bmpbits3;
     BYTE FAR* bmpbits4;
     BYTE FAR* bmpbits5;
     BYTE FAR* bmpbits6;
     int x;
     int BitCount = 24;
 
     width = width * (width > 0 ? 1 : -1 );
     height = height * (height > 0 ? 1 : -1 );
        
     /*temp[0] = CreateCompatibleBitmap(hsrcdc, width, height);
     temp[1] = CreateCompatibleBitmap(hsrcdc, width, height);
     temp[2] = CreateCompatibleBitmap(hsrcdc, width, height);*/
     
     BITMAPINFOHEADER generalheader;
     BITMAPINFO bmiinfo;
     
     generalheader.biWidth = width;
     generalheader.biHeight = height;
     generalheader.biSize = sizeof(BITMAPINFOHEADER);
     generalheader.biCompression = BI_RGB;
     generalheader.biBitCount = 24;
     generalheader.biPlanes = 1;
     generalheader.biSizeImage = 0;
     generalheader.biXPelsPerMeter = 0;
     generalheader.biYPelsPerMeter = 0;
     generalheader.biClrUsed = 0;
     generalheader.biClrImportant = 0;   
     
     tempdc[0] = CreateCompatibleDC(hsrcdc);
     tempdc[1] = CreateCompatibleDC(hsrcdc);
     tempdc[2] = CreateCompatibleDC(hsrcdc);
     
     /*Allocate memory for image bits*/ 
     bmpbits1 = malloc( sizeof(BYTE)*4*width*height);
     bmpbits2 = malloc( sizeof(BYTE)*4*width*height);
     bmpbits3 = malloc( sizeof(BYTE)*4*width*height);
     bmpbits4 = malloc( sizeof(BYTE)*4*width*height);
     bmpbits5 = malloc( sizeof(BYTE)*4*width*height);
     bmpbits6 = malloc( sizeof(BYTE)*4*width*height);

     memcpy(&bmiinfo.bmiHeader, &generalheader, sizeof(BITMAPINFOHEADER));
     
     if(bmpbits1 != NULL)
        temp[0] = CreateDIBitmap(tempdc[0], &generalheader, 2, bmpbits1, &bmiinfo, DIB_RGB_COLORS);
     if(bmpbits2 != NULL)
        temp[1] = CreateDIBitmap(tempdc[1], &generalheader, 2, bmpbits2, &bmiinfo, DIB_RGB_COLORS);
     if(bmpbits3 != NULL)
        temp[2] = CreateDIBitmap(tempdc[2], &generalheader, 2, bmpbits3, &bmiinfo, DIB_RGB_COLORS);
     
    
     old[0] = SelectObject(tempdc[0] ,temp[0]);
     old[1] = SelectObject(tempdc[1] ,temp[1]);
     old[2] = SelectObject(tempdc[2] ,temp[2]);
     
     
     BitBlt(tempdc[0], 0,0,width,height,hdestdc, destx, desty, SRCCOPY);
     BitBlt(tempdc[1], 0,0,width,height,hsrcdc, srcx, srcy, SRCCOPY);
     BitBlt(tempdc[2], 0,0,width,height,alphamaskdc, maskx, masky, SRCCOPY);



     SelectObject(tempdc[0], old[0]);
     SelectObject(tempdc[1], old[1]);
     SelectObject(tempdc[2], old[2]);
     
     if(bmpbits1 == NULL || bmpbits2 == NULL || bmpbits3 == NULL || bmpbits4 == NULL || bmpbits5 == NULL || bmpbits6 == NULL)
     {
        /*If malloc fails, clean up and abort*/
        if(bmpbits1 != NULL)
           free(bmpbits1);         
        if(bmpbits2 != NULL)
           free(bmpbits2);         
        if(bmpbits3 != NULL)
           free(bmpbits3);         
        if(bmpbits4 != NULL)
           free(bmpbits4);         
        if(bmpbits5 != NULL)
           free(bmpbits5);         
        if(bmpbits6 != NULL)
           free(bmpbits6);         
        
        DeleteObject(temp[0]);
        DeleteObject(temp[1]);
        DeleteObject(temp[2]);
        
        DeleteDC(tempdc[0]);
        DeleteDC(tempdc[1]);
        DeleteDC(tempdc[2]);        
     }
     else
     {
         /*GetDIBits(tempdc[0], temp[0], 0, height, bmpbits1, bmpinfoheader[0], DIB_RGB_COLORS);
         GetDIBits(tempdc[1], temp[1], 0, height, bmpbits2, bmpinfoheader[1], DIB_RGB_COLORS);
         GetDIBits(tempdc[2], temp[2], 0, height, bmpbits3, bmpinfoheader[2], DIB_RGB_COLORS);*/
         GetBitmapBits(temp[0], width*height*4, bmpbits4);
         GetBitmapBits(temp[1], width*height*4, bmpbits5);
         GetBitmapBits(temp[2], width*height*4, bmpbits6);
         
         for(x = 0; x < (4*width*height); x++ )
         { 
             /*Dst.Red 	= Src.Red * (SCA/255.0) 	+ Dst.Red * (1.0 - (SCA/255.0))*/
             //bmpbits4[x] = (BYTE)((float)bmpbits4[x] + ((float)bmpbits5[x]*((float)bmpbits6[x]/255.0))>255 ? 255 : (float)bmpbits4[x] + ((float)bmpbits5[x]*((float)bmpbits6[x]/255.0))  ) + 30;
             bmpbits4[x] = (BYTE)((float)bmpbits4[x]*(1.0 - ((float)bmpbits6[x]/255.0)) + (float)bmpbits5[x]*((float)bmpbits6[x]/255.0));
         }
         /*SetDIBits(tempdc[0], temp[0], 0, height, bmpbits1, bmpinfoheader[0], DIB_RGB_COLORS);*/

     SetBitmapBits(temp[0], width*height*4, bmpbits4);        
    
     
     old[0] = SelectObject(tempdc[0] ,temp[0]);
     BitBlt(hdestdc,destx, desty, width, height, tempdc[0], 0,0,SRCCOPY );
     SelectObject(tempdc[0], old[0]);

     DeleteObject(temp[0]);
     DeleteObject(temp[1]);
     DeleteObject(temp[2]);

     if(bmpbits1 != NULL)
       free(bmpbits1);         
     if(bmpbits2 != NULL)
       free(bmpbits2);         
     if(bmpbits3 != NULL)
       free(bmpbits3);         
     if(bmpbits4 != NULL)
       free(bmpbits4);         
     if(bmpbits5 != NULL)
       free(bmpbits5);         
     if(bmpbits6 != NULL)
       free(bmpbits6);         
                
     DeleteDC(tempdc[0]);
     DeleteDC(tempdc[1]);
     DeleteDC(tempdc[2]);            
     }
}