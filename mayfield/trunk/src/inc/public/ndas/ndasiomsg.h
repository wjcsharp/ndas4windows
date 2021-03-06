/////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2004-2005 XIMETA, Inc.
// All rights reserved.
//
// Module Name:
//
//    ndasiomsg.h
//
// Abstract:
//
//    NDAS Error Messages and Event Log Messages
//
//Notes:
//
//	   DO NOT EDIT THIS FILE BY HAND!
//
//    This file is GENERATED from the 'ndasiomg.xml'.
//
//////////////////////////////////////////////////////////////////////////


#ifndef __NDASIOMSG_H_
#define __NDASIOMSG_H_

//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: NDASSCSI_IO_MEMBER_FAULT
//
// MessageText:
//
//  One member of %1 device(%2) entered fault state.
//
#define NDASSCSI_IO_MEMBER_FAULT         ((NTSTATUS)0xAE010101L)

//
// MessageId: NDASSCSI_IO_MEMBER_FAULT_RECOVERED
//
// MessageText:
//
//  Faulty device member of %1 device(%2) got recovered.
//
#define NDASSCSI_IO_MEMBER_FAULT_RECOVERED ((NTSTATUS)0x6E010102L)

//
// MessageId: NDASSCSI_IO_RECONNECT_START
//
// MessageText:
//
//  %1 device(%2) started to reconnect. The device may not response until reconnecting is finished.
//
#define NDASSCSI_IO_RECONNECT_START      ((NTSTATUS)0xAE010103L)

//
// MessageId: NDASSCSI_IO_RECONNECTED
//
// MessageText:
//
//  %1 device(%2) was reconnected successfully.
//
#define NDASSCSI_IO_RECONNECTED          ((NTSTATUS)0x6E010104L)

//
// MessageId: NDASSCSI_IO_RECOVERY_START
//
// MessageText:
//
//  %1 device(%2) started to recover broken devices. The device may not response quickly.
//
#define NDASSCSI_IO_RECOVERY_START       ((NTSTATUS)0xAE010105L)

//
// MessageId: NDASSCSI_IO_RECOVERED
//
// MessageText:
//
//  %1 device(%2) was recovered successfully.
//
#define NDASSCSI_IO_RECOVERED            ((NTSTATUS)0x6E010106L)

//
// MessageId: NDASSCSI_IO_UPGRADE_SUCC
//
// MessageText:
//
//  %1 device(%2) was given primary access right.
//
#define NDASSCSI_IO_UPGRADE_SUCC         ((NTSTATUS)0x6E010107L)

//
// MessageId: NDASSCSI_IO_UPGRADE_FAIL
//
// MessageText:
//
//  %1 device(%2) could not acquire primary access right.
//
#define NDASSCSI_IO_UPGRADE_FAIL         ((NTSTATUS)0x6E010108L)

//
// MessageId: NDASSCSI_IO_COMPIRP_FAIL
//
// MessageText:
//
//  %1 device(%2) could not complete an IO request in the fast completion method.
//
#define NDASSCSI_IO_COMPIRP_FAIL         ((NTSTATUS)0xEE010109L)

//
// MessageId: NDASSCSI_IO_INVALID_TARGETTYPE
//
// MessageText:
//
//  %1 device(%2) could not recognize the target type.
//
#define NDASSCSI_IO_INVALID_TARGETTYPE   ((NTSTATUS)0xEE01010AL)

//
// MessageId: NDASSCSI_IO_MEMALLOC_FAIL
//
// MessageText:
//
//  %1 device(%2) could not allocate memory.
//
#define NDASSCSI_IO_MEMALLOC_FAIL        ((NTSTATUS)0xEE01010BL)

//
// MessageId: NDASSCSI_IO_INVAILD_TARGETDATA
//
// MessageText:
//
//  %1 device(%2) received invalid target information.
//
#define NDASSCSI_IO_INVAILD_TARGETDATA   ((NTSTATUS)0xEE01010CL)

//
// MessageId: NDASSCSI_IO_FINDADAPTER_START
//
// MessageText:
//
//  %1 device(%2) started to find an SCSI adapter.
//
#define NDASSCSI_IO_FINDADAPTER_START    ((NTSTATUS)0x6E01010EL)

//
// MessageId: NDASSCSI_IO_ADAPTERENUM_FAIL
//
// MessageText:
//
//  %1 device(%2) could not retrieve information on an SCSI adapter.
//
#define NDASSCSI_IO_ADAPTERENUM_FAIL     ((NTSTATUS)0xEE01010FL)

//
// MessageId: NDASSCSI_IO_FIRSTINSTALL
//
// MessageText:
//
//  %1 device(%2) entered raw device mode.
//
#define NDASSCSI_IO_FIRSTINSTALL         ((NTSTATUS)0x6E010110L)

//
// MessageId: NDASSCSI_IO_RECV_LURDESC
//
// MessageText:
//
//  %1 device(%2) retrieved LUR descriptor.
//
#define NDASSCSI_IO_RECV_LURDESC         ((NTSTATUS)0x6E010111L)

//
// MessageId: NDASSCSI_IO_RECV_ADDTARGETDATA
//
// MessageText:
//
//  %1 device(%2) retrieved AddTargetData descriptor.
//
#define NDASSCSI_IO_RECV_ADDTARGETDATA   ((NTSTATUS)0x6E010112L)

//
// MessageId: NDASSCSI_IO_INITLUR_FAIL
//
// MessageText:
//
//  %1 device(%2) could not initialize a logical unit.
//
#define NDASSCSI_IO_INITLUR_FAIL         ((NTSTATUS)0xEE010113L)

//
// MessageId: NDASSCSI_IO_FINDADAPTER_SUCC
//
// MessageText:
//
//  %1 device(%2) found a SCSI adapter successfuly.
//
#define NDASSCSI_IO_FINDADAPTER_SUCC     ((NTSTATUS)0x6E010114L)

//
// MessageId: NDASSCSI_IO_BUSRESET_OCCUR
//
// MessageText:
//
//  %1 device(%2) reset the SCSI bus.
//
#define NDASSCSI_IO_BUSRESET_OCCUR       ((NTSTATUS)0xAE010115L)

//
// MessageId: NDASSCSI_IO_STOP_REQUESTED
//
// MessageText:
//
//  %1 device(%2) was requested to stop now.
//
#define NDASSCSI_IO_STOP_REQUESTED       ((NTSTATUS)0xAE010116L)

//
// MessageId: NDASSCSI_IO_INQUIRY_WHILE_STOPPING
//
// MessageText:
//
//  %1 device(%2) received inquiry request while stopping the device.
//
#define NDASSCSI_IO_INQUIRY_WHILE_STOPPING ((NTSTATUS)0xAE010119L)

//
// MessageId: NDASSCSI_IO_LUR_NOT_FOUND
//
// MessageText:
//
//  %1 device(%2) could not return logical unit information.
//
#define NDASSCSI_IO_LUR_NOT_FOUND        ((NTSTATUS)0xAE01011AL)

//
// MessageId: NDASSCSI_IO_NO_SHIPPED_SRB
//
// MessageText:
//
//  %1 device(%2) received a fast competion request, but it did not contain a SCSI request.
//
#define NDASSCSI_IO_NO_SHIPPED_SRB       ((NTSTATUS)0xAE01011EL)

//
// MessageId: NDASSCSI_IO_SRB_DISCARDED
//
// MessageText:
//
//  %1 device(%2) discarded an old SCSI request.
//
#define NDASSCSI_IO_SRB_DISCARDED        ((NTSTATUS)0x6E01011FL)

//
// MessageId: NDASSCSI_IO_ABORT_SRB
//
// MessageText:
//
//  %1 device(%2) was requested to abort a SCSI request.
//
#define NDASSCSI_IO_ABORT_SRB            ((NTSTATUS)0xAE010120L)

//
// MessageId: NDASSCSI_IO_CCBALLOC_FAIL
//
// MessageText:
//
//  %1 device(%2) could not allocate an internal request block.
//
#define NDASSCSI_IO_CCBALLOC_FAIL        ((NTSTATUS)0xEE010121L)

//
// MessageId: NDASSCSI_IO_STOPIOCTL_WHILESTOPPING
//
// MessageText:
//
//  %1 device(%2) received Stop Ioctl while stopping.
//
#define NDASSCSI_IO_STOPIOCTL_WHILESTOPPING ((NTSTATUS)0xAE010122L)

//
// MessageId: NDASSCSI_IO_STOPIOCTL_NO_LUR
//
// MessageText:
//
//  %1 device(%2) has no logical unit to stop.
//
#define NDASSCSI_IO_STOPIOCTL_NO_LUR     ((NTSTATUS)0xAE010123L)

//
// MessageId: NDASSCSI_IO_STOPIOCTL_LUR_ERROR
//
// MessageText:
//
//  %1 device(%2) failed stop ioctl. could not send stop request to a logical unit.
//
#define NDASSCSI_IO_STOPIOCTL_LUR_ERROR  ((NTSTATUS)0xEE010124L)

//
// MessageId: NDASSCSI_IO_UPGRADEIOCTL_FAIL
//
// MessageText:
//
//  %1 device(%2) failed primary ioctl.
//
#define NDASSCSI_IO_UPGRADEIOCTL_FAIL    ((NTSTATUS)0xAE010125L)

//
// MessageId: NDASSCSI_IO_STOPIOCTL_INVALIDLUR
//
// MessageText:
//
//  %1 device(%2) received Stop Ioctl with invalid LUR
//
#define NDASSCSI_IO_STOPIOCTL_INVALIDLUR ((NTSTATUS)0xAE010126L)

//
// MessageId: NDASBUS_IO_PDO_NOT_FOUND
//
// MessageText:
//
//  %1 could not find the specified device(%2).
//
#define NDASBUS_IO_PDO_NOT_FOUND         ((NTSTATUS)0xAE020101L)

//
// MessageId: NDASBUS_IO_REGISTER_TARGET_FAIL
//
// MessageText:
//
//  %1 could not register a target data of device(%2).
//
#define NDASBUS_IO_REGISTER_TARGET_FAIL  ((NTSTATUS)0xAE020102L)

//
// MessageId: NDASBUS_IO_UNREGISTER_TARGET_FAIL
//
// MessageText:
//
//  %1 could not unregister a target data of device(%2).
//
#define NDASBUS_IO_UNREGISTER_TARGET_FAIL ((NTSTATUS)0xAE020103L)

//
// MessageId: NDASBUS_IO_TRY_TO_ADDTARGET
//
// MessageText:
//
//  %1 tried to add a target to device(%2).
//
#define NDASBUS_IO_TRY_TO_ADDTARGET      ((NTSTATUS)0x6E020104L)

#endif /* __NDASIOMSG_H_ */
