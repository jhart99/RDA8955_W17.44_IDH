/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/

#ifndef _IOT_SMS_H
#define _IOT_SMS_H
//BOOL CFW_SmsInitReady(CFW_SIM_ID nSimID);

class IOT_SMS
{
public:
    IOT_SMS();
    bool ready();
    bool available();
    bool remoteNumber(char *nBuf, int nLen);
    int  read();
    bool send_sms(char *number, char *sms);
    bool handler_sms(char *number, char *sms);
    bool flush();
private:
    int index;
};

#endif

