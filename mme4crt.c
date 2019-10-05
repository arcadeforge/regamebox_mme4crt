/***/
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

long free_pixel_clock = 31270000;
int compute_dynamic_width(int width, float freq);

void crt_rpi_switch(int width, int height, float hz, int crt_center_adjust);

int main(int argc, char **argv)
{


    if (argc != 6)
    {
        printf("Usage : mme4crt <width> <heigth> <freq> <shift> <superres> \n");
        printf("      : width of game\n");
        printf("      : heigth of game\n");
        printf("      : freq of game\n");
        printf("      : shift in x\n");
        printf("      : 1 or 0 for superres\n");
        printf("mme4crt 320 224 60 1\n");
        return 0;
    }

    int w, h, shift, superres = 0;
    float freq = 0;
    w = strtol(argv[1], NULL, 10);
    h = strtol(argv[2], NULL, 10);
    freq = strtof(argv[3], NULL);
    shift = strtol(argv[4], NULL, 10);
    superres = strtol(argv[5], NULL, 10);

    printf ("width : %i\n", w);
    printf ("height : %i\n", h);
    printf ("freq : %f\n", freq);
    printf ("shift : %i\n", shift );
    printf ("super res : %i\n", superres);

    printf ("dynamic width : %i\n", compute_dynamic_width (w, freq));

    if (superres == 1)
        crt_rpi_switch (compute_dynamic_width (w, freq), h, freq, shift);
    else
        crt_rpi_switch (w, h, freq, shift);
}


int compute_dynamic_width(int width, float freq)
{
    float i;
    int dynamic_width   = 0;
    int min_height = 261;

    long p_clock_test = 0;
   
   for (i = 0; i < 10; i=i+0.1)
   {
        dynamic_width = width * i;
        p_clock_test = dynamic_width * min_height * freq;
/*
        printf ("min_height : %i\n",min_height );
        printf ("freq : %i\n", freq);
        printf ("dynamic width : %i\n", dynamic_width);
        printf ("p_clock : %i\n", p_clock);
        printf ("p_clock_test : %i\n", p_clock_test);

*/
        if (p_clock_test > free_pixel_clock)
            break;

   }
   return dynamic_width;
}

void crt_rpi_switch(int width, int height, float hz, int crt_center_adjust)
{

    char buffer[1024];
    static char output[250]             = {0};
    static char output1[250]            = {0};
    static char output2[250]            = {0};
    static char set_hdmi[250]           = {0};
    static char set_hdmi_timing[250]    = {0};
    int i              = 0;
    int hfp            = 0;
    int hsp            = 0;
    int hbp            = 0;
    int vfp            = 0;
    int vsp            = 0;
    int vbp            = 0;
    int hmax           = 0;
    int vmax           = 0;
    int pdefault       = 8;
    int pwidth         = 0;
    float roundw     = 0.0f;
    float roundh     = 0.0f;
    float pixel_clock  = 0;
    int ip_flag     = 0;
    int xoffset = crt_center_adjust;
    pid_t pid = fork();



    /* following code is the mode line generator */
    hsp = (width * 0.117) - (xoffset*4);
    if (width < 700)
    {
        hfp    = (width * 0.065);
        hbp  = width * 0.35-hsp-hfp;
    }
    else
    {
        hfp  = (width * 0.033) + (width / 112);
        hbp  = (width * 0.225) + (width /58);
        xoffset = xoffset*2;
    }

    hmax = hbp;

    if (height < 241)
        vmax = 261;
    if (height < 241 && hz > 56 && hz < 58)
        vmax = 280;
    if (height < 241 && hz < 55)
        vmax = 313;
    if (height > 250 && height < 260 && hz > 54)
        vmax = 296;
    if (height > 250 && height < 260 && hz > 52 && hz < 54)
        vmax = 285;
    if (height > 250 && height < 260 && hz < 52)
        vmax = 313;
    if (height > 260 && height < 300)
        vmax = 318;

    if (height > 400 && hz > 56)
        vmax = 533;
    if (height > 520 && hz < 57)
        vmax = 580;

    if (height > 300 && hz < 56)
         vmax = 615;
    if (height > 500 && hz < 56)
        vmax = 624;
    if (height > 300)
        pdefault = pdefault * 2;

    vfp = (height + ((vmax - height) / 2) - pdefault) - height;

    if (height < 300)
        vsp = vfp + 3; /* needs to be 3 for progressive */
    if (height > 300)
        vsp = vfp + 6; /* needs to be 6 for interlaced */

    vsp = 3;

    vbp = (vmax-height)-vsp-vfp;

    hmax = width+hfp+hsp+hbp;

    if (height < 300)
    {
        pixel_clock = (hmax * vmax * hz) ;
        ip_flag     = 0;
    }

    if (height > 300)
    {
        pixel_clock = (hmax * vmax * hz) /2 ;
        ip_flag     = 1;
    }
    /* above code is the modeline generator */

    if (pixel_clock < free_pixel_clock)
        if (pixel_clock < 5600000)
            pixel_clock = 4800000;
        else
            if (pixel_clock < 8000000)
                pixel_clock = 6400000;
            else
                if (pixel_clock < 14400000)
                    pixel_clock = 9600000;
                else
                    pixel_clock = 19200000;



    snprintf(set_hdmi_timing, sizeof(set_hdmi_timing),
        "vcgencmd hdmi_timings %d 1 %d %d %d %d 1 %d %d %d 0 0 0 %f %d %f 1",
        width, hfp, hsp, hbp, height, vfp,vsp, vbp,
        hz, ip_flag, pixel_clock);

    FILE *f = fopen("timings.txt", "a");
    fprintf(f,"%s\n",set_hdmi_timing);
    fclose(f);

    printf("%s\n",set_hdmi_timing);


    if (pid == 0) 
    {
        system(set_hdmi_timing);
        sleep(0.5);
        snprintf(output1,  sizeof(output1),
           "tvservice -e \"DMT 87\"");
        system(output1);

        sleep(0.3);
        snprintf(output2,  sizeof(output1),
          "fbset -g %d %d %d %d 32", width, height, width, height);
        system(output2);
        //snprintf(output2,  sizeof(output1),
        //   "fbset -depth 32");
        //system(output2);
        exit(0);
   }

}
