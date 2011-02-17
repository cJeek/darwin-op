#include <stdio.h>
#include <string.h>
#include "cmd_process.h"

using namespace Robot;


const char *GetIDString(int id)
{
	switch(id)
	{
	case JointData::ID_R_SHOULDER_PITCH:
		return "R_SHOULDER_PITCH";

	case JointData::ID_L_SHOULDER_PITCH:
		return "L_SHOULDER_PITCH";

	case JointData::ID_R_SHOULDER_ROLL:
		return "R_SHOULDER_ROLL";

	case JointData::ID_L_SHOULDER_ROLL:
		return "L_SHOULDER_ROLL";

	case JointData::ID_R_ELBOW:
		return "R_ELBOW";

	case JointData::ID_L_ELBOW:
		return "L_ELBOW";

	case JointData::ID_R_HIP_YAW:
		return "R_HIP_YAW";

	case JointData::ID_L_HIP_YAW:
		return "L_HIP_YAW";

	case JointData::ID_R_HIP_ROLL:
		return "R_HIP_ROLL";

	case JointData::ID_L_HIP_ROLL:
		return "L_HIP_ROLL";

	case JointData::ID_R_HIP_PITCH:
		return "R_HIP_PITCH";

	case JointData::ID_L_HIP_PITCH:
		return "L_HIP_PITCH";

	case JointData::ID_R_KNEE:
		return "R_KNEE";

	case JointData::ID_L_KNEE:
		return "L_KNEE";

	case JointData::ID_R_ANKLE_PITCH:
		return "R_ANKLE_PITCH";

	case JointData::ID_L_ANKLE_PITCH:
		return "L_ANKLE_PITCH";

	case JointData::ID_R_ANKLE_ROLL:
		return "R_ANKLE_ROLL";

	case JointData::ID_L_ANKLE_ROLL:
		return "L_ANKLE_ROLL";

	case JointData::ID_HEAD_PAN:
		return "HEAD_PAN";

	case JointData::ID_HEAD_TILT:
		return "HEAD_TILT";

	case CM730::ID_CM:
		return "SUB_BOARD";
	}

	return "UNKNOWN";
}

void Prompt(int id)
{
	printf( "\r[ID:%d(%s)] ", id, GetIDString(id) );
}

void Help()
{
	printf( "\n" );
	printf( " exit : Terminate program\n" );
	printf( " scan : Check connection of dynamixel\n" );
	printf( " id [ID] : Change selected [ID]\n" );
	printf( " dump : Dump control table of Dynamixel\n" );
	printf( " reset : Set default value for selected ID's Dynamixel\n" );
	printf( " reset all : Set default value for all Dynamixel\n" );
	printf( " wr [ADDR] [VALUE] : Write [VALUE] to [ADDR] of selected ID's Dynamixel\n" );
	printf( " on/off : If actuator, torque on/off\n" );
	printf( "          If sub-board, actuator power on/off\n" );
	printf( " on/off all : Torque on/off all actuator)\n" );
	printf( "\n       Copyright ROBOTIS CO.,LTD.\n\n" );
}

void Scan(CM730 *cm730)
{
	printf("\n");

	for(int id=JointData::ID_R_SHOULDER_PITCH; id<JointData::NUMBER_OF_JOINTS; id++)
	{
		printf(" Check ID:%d(%s) ...", id, GetIDString(id));
		if(cm730->Ping(id, 0) == CM730::SUCCESS)
			printf("OK\n");
		else
			printf("FAIL\n");
	}

	printf(" Check ID:%d(%s) ...", CM730::ID_CM, GetIDString(CM730::ID_CM));
	if(cm730->Ping(CM730::ID_CM, 0) == CM730::SUCCESS)
		printf("OK\n");
	else
		printf("FAIL\n");

	printf("\n");
}

void Dump(CM730 *cm730, int id)
{
	unsigned char table[128];
	int addr;
	int value;


	if(id == CM730::ID_CM) // Sub board
	{
		if(cm730->ReadTable(id, CM730::P_MODEL_NUMBER_L, CM730::P_ACCEL_Z_H, &table[CM730::P_MODEL_NUMBER_L], 0) != CM730::SUCCESS)
		{
			printf(" Can not read table!\n");
			return;
		}

		printf( "\n" );
		printf( " [EEPROM AREA]\n" );
		addr = CM730::P_MODEL_NUMBER_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " MODEL_NUMBER            (R) [%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = CM730::P_VERSION; value = table[addr];
		printf( " VERSION                 (R) [%.3d]:%5d\n", addr, value);
		addr = CM730::P_ID; value = table[addr];
		printf( " ID                     (R/W)[%.3d]:%5d\n", addr, value);
		addr = CM730::P_BAUD_RATE; value = table[addr];
		printf( " BAUD_RATE              (R/W)[%.3d]:%5d\n", addr, value);
		addr = CM730::P_RETURN_DELAY_TIME; value = table[addr];
		printf( " RETURN_DELAY_TIME      (R/W)[%.3d]:%5d\n", addr, value);
		addr = CM730::P_RETURN_LEVEL; value = table[addr];
		printf( " RETURN_LEVEL           (R/W)[%.3d]:%5d\n", addr, value);
		printf( "\n" );
		printf( " [RAM AREA]\n" );
		addr = CM730::P_DXL_POWER; value = table[addr];
		printf( " DXL_POWER              (R/W)[%.3d]:%5d\n", addr, value);
		addr = CM730::P_LED_PANNEL; value = table[addr];
		printf( " LED_PANNEL             (R/W)[%.3d]:%5d\n", addr, value);
		addr = CM730::P_LED_HEAD_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " LED_HEAD               (R/W)[%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = CM730::P_LED_EYE_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " LED_EYE                (R/W)[%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = CM730::P_BUTTON; value = table[addr];
		printf( " BUTTON                  (R) [%.3d]:%5d\n", addr, value);
		addr = CM730::P_VOLTAGE; value = table[addr];
		printf( " VOLTAGE                 (R) [%.3d]:%5d\n", addr, value);
		addr = CM730::P_GYRO_Z_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " P_GYRO_Z                (R) [%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = CM730::P_GYRO_Y_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " P_GYRO_Y                (R) [%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = CM730::P_GYRO_X_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " P_GYRO_X                (R) [%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = CM730::P_ACCEL_X_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " P_ACCEL_X               (R) [%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = CM730::P_ACCEL_Y_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " P_ACCEL_Y               (R) [%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = CM730::P_ACCEL_Z_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " P_ACCEL_Z               (R) [%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = CM730::P_LEFT_MIC_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " P_LEFT_MIC              (R) [%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = CM730::P_RIGHT_MIC_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " P_RIGHT_MIC             (R) [%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);

		printf( "\n" );
	}
	else // Actuator
	{		
		if(cm730->ReadTable(id, RX28M::P_MODEL_NUMBER_L, RX28M::P_PUNCH_H, &table[RX28M::P_MODEL_NUMBER_L], 0) != CM730::SUCCESS)
		{
			printf(" Can not read table!\n");
			return;
		}

		printf( "\n" );
		printf( " [EEPROM AREA]\n" );
		addr = RX28M::P_MODEL_NUMBER_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " MODEL_NUMBER            (R) [%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = RX28M::P_VERSION; value = table[addr];
		printf( " VERSION                 (R) [%.3d]:%5d\n", addr, value);
		addr = RX28M::P_ID; value = table[addr];
		printf( " ID                     (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_BAUD_RATE; value = table[addr];
		printf( " BAUD_RATE              (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_RETURN_DELAY_TIME; value = table[addr];
		printf( " RETURN_DELAY_TIME      (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_CW_ANGLE_LIMIT_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " CW_ANGLE_LIMIT         (R/W)[%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = RX28M::P_CCW_ANGLE_LIMIT_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " CCW_ANGLE_LIMIT        (R/W)[%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = RX28M::P_HIGH_LIMIT_TEMPERATURE; value = table[addr];
		printf( " HIGH_LIMIT_TEMPERATURE (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_LOW_LIMIT_VOLTAGE; value = table[addr];
		printf( " LOW_LIMIT_VOLTAGE      (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_HIGH_LIMIT_VOLTAGE; value = table[addr];
		printf( " HIGH_LIMIT_VOLTAGE     (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_MAX_TORQUE_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " MAX_TORQUE             (R/W)[%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = RX28M::P_RETURN_LEVEL; value = table[addr];
		printf( " RETURN_LEVEL           (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_ALARM_LED; value = table[addr];
		printf( " ALARM_LED              (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_ALARM_SHUTDOWN; value = table[addr];
		printf( " ALARM_SHUTDOWN         (R/W)[%.3d]:%5d\n", addr, value);
		printf( "\n" );
		printf( " [RAM AREA]\n" );
		addr = RX28M::P_TORQUE_ENABLE; value = table[addr];
		printf( " TORQUE_ENABLE          (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_LED; value = table[addr];
		printf( " LED                    (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_CW_COMPLIANCE_MARGIN; value = table[addr];
		printf( " CW_COMPLIANCE_MARGIN   (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_CCW_COMPLIANCE_MARGIN; value = table[addr];
		printf( " CCW_COMPLIANCE_MARGIN  (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_CW_COMPLIANCE_SLOPE; value = table[addr];
		printf( " CW_COMPLIANCE_SLOPE    (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_CCW_COMPLIANCE_SLOPE; value = table[addr];
		printf( " CCW_COMPLIANCE_SLOPE   (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_GOAL_POSITION_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " GOAL_POSITION          (R/W)[%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = RX28M::P_MOVING_SPEED_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " MOVING_SPEED           (R/W)[%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = RX28M::P_TORQUE_LIMIT_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " TORQUE_LIMIT           (R/W)[%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = RX28M::P_PRESENT_POSITION_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " PRESENT_POSITION       (R/W)[%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = RX28M::P_PRESENT_SPEED_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " PRESENT_SPEED          (R/W)[%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = RX28M::P_PRESENT_LOAD_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " PRESENT_LOAD           (R/W)[%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);
		addr = RX28M::P_PRESENT_VOLTAGE; value = table[addr];
		printf( " PRESENT_VOLTAGE        (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_PRESENT_TEMPERATURE; value = table[addr];
		printf( " PRESENT_TEMPERATURE    (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_REGISTERED_INSTRUCTION; value = table[addr];
		printf( " REGISTERED_INSTRUC     (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_MOVING; value = table[addr];
		printf( " MOVING                 (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_LOCK; value = table[addr];
		printf( " LOCK                   (R/W)[%.3d]:%5d\n", addr, value);
		addr = RX28M::P_PUNCH_L; value = CM730::MakeWord(table[addr], table[addr+1]);
		printf( " PUNCH                  (R/W)[%.3d]:%5d (L:0x%.2X H:0x%.2X)\n", addr, value, table[addr], table[addr+1]);

		printf( "\n" );
	}
}

void Reset(Robot::CM730 *cm730, int id)
{
	int FailCount = 0;
	int FailMaxCount = 10;
	printf(" Reset ID:%d...", id);

	if(cm730->Ping(id, 0) != CM730::SUCCESS)
	{
		printf("Fail\n");
		return;
	}

	FailCount = 0;
	while(1)
	{
		if(cm730->WriteByte(id, RX28M::P_RETURN_DELAY_TIME, 0, 0) == CM730::SUCCESS)
			break;

		FailCount++;
		if(FailCount > FailMaxCount)
		{
			printf("Fail\n");
			return;
		}
		usleep(10000);
	}

	FailCount = 0;
	while(1)
	{
		if(cm730->WriteByte(id, RX28M::P_RETURN_LEVEL, 2, 0) == CM730::SUCCESS)
			break;

		FailCount++;
		if(FailCount > FailMaxCount)
		{
			printf("Fail\n");
			return;
		}
		usleep(10000);
	}

	if(id != CM730::ID_CM)
	{
		int cwLimit = RX28M::MIN_VALUE;
		int ccwLimit = RX28M::MAX_VALUE;

		switch(id)
		{
		case JointData::ID_R_SHOULDER_ROLL:
			cwLimit = 237;
			ccwLimit = 962;
			break;

		case JointData::ID_L_SHOULDER_ROLL:
			cwLimit = 51;
			ccwLimit = 765;
			break;

		case JointData::ID_R_ELBOW:
			cwLimit = 186;
			ccwLimit = 742;
			break;

		case JointData::ID_L_ELBOW:
			cwLimit = 270;
			ccwLimit = 832;
			break;

		case JointData::ID_R_HIP_YAW:
            cwLimit = 95;
            ccwLimit = 689;
            break;

		case JointData::ID_L_HIP_YAW:
			cwLimit = 330;
			ccwLimit = 805;
			break;

		case JointData::ID_R_HIP_ROLL:
			cwLimit = 356;
			ccwLimit = 712;
			break;

		case JointData::ID_L_HIP_ROLL:
			cwLimit = 314;
			ccwLimit = 652;
			break;

		case JointData::ID_R_HIP_PITCH:
			cwLimit = 177;
			ccwLimit = 607;
			break;

		case JointData::ID_L_HIP_PITCH:
			cwLimit = 414;
			ccwLimit = 854;
			break;

		case JointData::ID_R_KNEE:
			cwLimit = 492;
			ccwLimit = 952;
			break;

		case JointData::ID_L_KNEE:
			cwLimit = 67;
			ccwLimit = 532;
			break;

		case JointData::ID_R_ANKLE_PITCH:
			cwLimit = 312;
			ccwLimit = 774;
			break;

		case JointData::ID_L_ANKLE_PITCH:
			cwLimit = 240;
			ccwLimit = 756;
			break;

		case JointData::ID_R_ANKLE_ROLL:
			cwLimit = 362;
			ccwLimit = 712;
			break;

		case JointData::ID_L_ANKLE_ROLL:
			cwLimit = 298;
			ccwLimit = 662;
			break;

		case JointData::ID_HEAD_TILT:
			cwLimit = 420;
			ccwLimit = 708;
			break;
		}
		
		FailCount = 0;
		while(1)
		{
			if(cm730->WriteWord(id, RX28M::P_CW_ANGLE_LIMIT_L, cwLimit, 0) == CM730::SUCCESS)
				break;

			FailCount++;
			if(FailCount > FailMaxCount)
			{
				printf("Fail\n");
				return;
			}
			usleep(10000);
		}		
		FailCount = 0;
		while(1)
		{
			if(cm730->WriteWord(id, RX28M::P_CCW_ANGLE_LIMIT_L, ccwLimit, 0) == CM730::SUCCESS)
				break;

			FailCount++;
			if(FailCount > FailMaxCount)
			{
				printf("Fail\n");
				return;
			}
			usleep(10000);
		}		
		FailCount = 0;
		while(1)
		{
			if(cm730->WriteByte(id, RX28M::P_HIGH_LIMIT_TEMPERATURE, 80, 0) == CM730::SUCCESS)
				break;

			FailCount++;
			if(FailCount > FailMaxCount)
			{
				printf("Fail\n");
				return;
			}
			usleep(10000);
		}
		FailCount = 0;
		while(1)
		{
			if(cm730->WriteByte(id, RX28M::P_LOW_LIMIT_VOLTAGE, 60, 0) == CM730::SUCCESS)
				break;

			FailCount++;
			if(FailCount > FailMaxCount)
			{
				printf("Fail\n");
				return;
			}
			usleep(10000);
		}		
		FailCount = 0;
		while(1)
		{
			if(cm730->WriteByte(id, RX28M::P_HIGH_LIMIT_VOLTAGE, 140, 0) == CM730::SUCCESS)
				break;

			FailCount++;
			if(FailCount > FailMaxCount)
			{
				printf("Fail\n");
				return;
			}
			usleep(10000);
		}
		FailCount = 0;
		while(1)
		{
			if(cm730->WriteWord(id, RX28M::P_MAX_TORQUE_L, RX28M::MAX_VALUE, 0) == CM730::SUCCESS)
				break;

			FailCount++;
			if(FailCount > FailMaxCount)
			{
				printf("Fail\n");
				return;
			}
			usleep(10000);
		}
		FailCount = 0;
		while(1)
		{
			if(cm730->WriteByte(id, RX28M::P_ALARM_LED, 36, 0) == CM730::SUCCESS) // Overload, Overheat
				break;

			FailCount++;
			if(FailCount > FailMaxCount)
			{
				printf("Fail\n");
				return;
			}
			usleep(10000);
		}		 
		FailCount = 0;
		while(1)
		{
			if(cm730->WriteByte(id, RX28M::P_ALARM_SHUTDOWN, 36, 0) == CM730::SUCCESS) // Overload, Overheat
				break;

			FailCount++;
			if(FailCount > FailMaxCount)
			{
				printf("Fail\n");
				return;
			}
			usleep(10000);
		}		 
	}

	printf("Success\n");
}

void Write(Robot::CM730 *cm730, int id, int addr, int value)
{
	if(addr == RX28M::P_ID || addr == RX28M::P_BAUD_RATE || addr == RX28M::P_RETURN_DELAY_TIME || addr == RX28M::P_RETURN_LEVEL)
	{
		printf( " Can not change this address[%d]\n", addr);
		return;
	}

	int error = 0;
	int res;
	if(id == CM730::ID_CM)
	{
		if(addr >= CM730::MAXNUM_ADDRESS)
		{
			printf( " Invalid address\n");
			return;
		}

		if(addr == CM730::P_DXL_POWER
			|| addr == CM730::P_LED_PANNEL)
		{
			res = cm730->WriteByte(addr, value, &error);
		}
		else
		{
			res = cm730->WriteWord(addr, value, &error);
		}
	}
	else
	{
		if(addr >= RX28M::MAXNUM_ADDRESS)
		{
			printf( " Invalid address\n");
			return;
		}

		if(addr == RX28M::P_HIGH_LIMIT_TEMPERATURE
			|| addr == RX28M::P_LOW_LIMIT_VOLTAGE
			|| addr == RX28M::P_HIGH_LIMIT_VOLTAGE
			|| addr == RX28M::P_ALARM_LED
			|| addr == RX28M::P_ALARM_SHUTDOWN
			|| addr == RX28M::P_TORQUE_ENABLE
			|| addr == RX28M::P_LED
			|| addr == RX28M::P_CW_COMPLIANCE_MARGIN
			|| addr == RX28M::P_CCW_COMPLIANCE_MARGIN
			|| addr == RX28M::P_CW_COMPLIANCE_SLOPE
			|| addr == RX28M::P_CCW_COMPLIANCE_SLOPE
			|| addr == RX28M::P_LED
			|| addr == RX28M::P_LED)
		{
			res = cm730->WriteByte(id, addr, value, &error);
		}
		else
		{
			res = cm730->WriteWord(id, addr, value, &error);
		}
	}

	if(res != CM730::SUCCESS)
	{
		printf( " Fail to write!\n");
		return;
	}

	if(error != 0)
	{
		printf( " Access or range error!\n");
		return;
	}

	printf(" Success to write!\n");
}