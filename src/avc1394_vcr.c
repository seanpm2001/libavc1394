
#include "avc1394_vcr.h"
#include "avc1394.h"

#define AVC1394_RETRY 4

#define CTLVCR0 AVC1394_CTYPE_CONTROL | AVC1394_SUBUNIT_TYPE_TAPE_RECORDER | AVC1394_SUBUNIT_ID_0
#define STATVCR0 AVC1394_CTYPE_STATUS | AVC1394_SUBUNIT_TYPE_TAPE_RECORDER | AVC1394_SUBUNIT_ID_0
#define CTLTUNER0 AVC1394_CTYPE_CONTROL | AVC1394_SUBUNIT_TYPE_TUNER | AVC1394_SUBUNIT_ID_0
#define STATTUNER0 AVC1394_CTYPE_STATUS | AVC1394_SUBUNIT_TYPE_TUNER | AVC1394_SUBUNIT_ID_0
#define TUNER0 AVC1394_SUBUNIT_TYPE_TUNER | AVC1394_SUBUNIT_ID_0
#define CTLUNIT AVC1394_CTYPE_CONTROL | AVC1394_SUBUNIT_TYPE_UNIT | AVC1394_SUBUNIT_ID_IGNORE
#define STATUNIT AVC1394_CTYPE_STATUS | AVC1394_SUBUNIT_TYPE_UNIT | AVC1394_SUBUNIT_ID_IGNORE

int avc1394_vcr_is_playing(raw1394handle_t handle, nodeid_t node)
{

	quadlet_t response = avc1394_transaction(handle, node, STATVCR0
		| AVC1394_VCR_COMMAND_TRANSPORT_STATE | AVC1394_VCR_OPERAND_TRANSPORT_STATE,
		AVC1394_RETRY);
	if (AVC1394_MASK_OPCODE(response)
		== AVC1394_VCR_RESPONSE_TRANSPORT_STATE_PLAY)
		return AVC1394_GET_OPERAND0(response);
	else
		return 0;
}


int avc1394_vcr_is_recording(raw1394handle_t handle, nodeid_t node)
{

	quadlet_t response = avc1394_transaction(handle, node, STATVCR0
		| AVC1394_VCR_COMMAND_TRANSPORT_STATE | AVC1394_VCR_OPERAND_TRANSPORT_STATE,
		AVC1394_RETRY);
	if (AVC1394_MASK_OPCODE(response)
		== AVC1394_VCR_RESPONSE_TRANSPORT_STATE_RECORD)
		return AVC1394_GET_OPERAND0(response);
	else
		return 0;
}


void avc1394_vcr_play(raw1394handle_t handle, nodeid_t node)
{
	if (avc1394_vcr_is_playing(handle, node) == AVC1394_VCR_OPERAND_PLAY_FORWARD) {
		avc1394_send_command(handle, node, CTLVCR0
			| AVC1394_VCR_COMMAND_PLAY | AVC1394_VCR_OPERAND_PLAY_SLOWEST_FORWARD);
	} else {
		avc1394_send_command(handle, node, CTLVCR0
			| AVC1394_VCR_COMMAND_PLAY | AVC1394_VCR_OPERAND_PLAY_FORWARD);
	}
}


void avc1394_vcr_stop(raw1394handle_t handle, nodeid_t node)
{
	avc1394_send_command(handle, node, CTLVCR0
		| AVC1394_VCR_COMMAND_WIND | AVC1394_VCR_OPERAND_WIND_STOP);
}


void avc1394_vcr_rewind(raw1394handle_t handle, nodeid_t node)
{
	if (avc1394_vcr_is_playing(handle, node)) {
		avc1394_send_command(handle, node, CTLVCR0
			| AVC1394_VCR_COMMAND_PLAY | AVC1394_VCR_OPERAND_PLAY_FASTEST_REVERSE);

	} else {
		avc1394_send_command(handle, node, CTLVCR0
			| AVC1394_VCR_COMMAND_WIND | AVC1394_VCR_OPERAND_WIND_REWIND);

	}
}


void avc1394_vcr_pause(raw1394handle_t handle, nodeid_t node)
{
	int mode;

	
	if ((mode = avc1394_vcr_is_recording(handle, node))) {
		if (mode == AVC1394_VCR_OPERAND_RECORD_PAUSE) {
			avc1394_send_command(handle, node, CTLVCR0
			| AVC1394_VCR_COMMAND_RECORD | AVC1394_VCR_OPERAND_RECORD_RECORD);
		} else {
			avc1394_send_command(handle, node, CTLVCR0
			| AVC1394_VCR_COMMAND_RECORD | AVC1394_VCR_OPERAND_RECORD_PAUSE);
		}
	} else {
		if (avc1394_vcr_is_playing(handle, node)==AVC1394_VCR_OPERAND_PLAY_FORWARD_PAUSE) {
			avc1394_send_command(handle, node, CTLVCR0
			| AVC1394_VCR_COMMAND_PLAY | AVC1394_VCR_OPERAND_PLAY_FORWARD);
		} else {
			avc1394_send_command(handle, node, CTLVCR0
			| AVC1394_VCR_COMMAND_PLAY | AVC1394_VCR_OPERAND_PLAY_FORWARD_PAUSE);
		}
	}

}


void avc1394_vcr_forward(raw1394handle_t handle, nodeid_t node)
{
	if (avc1394_vcr_is_playing(handle, node)) {
		avc1394_send_command(handle, node, CTLVCR0
			| AVC1394_VCR_COMMAND_PLAY | AVC1394_VCR_OPERAND_PLAY_FASTEST_FORWARD);
	} else {
		avc1394_send_command(handle, node, CTLVCR0
			| AVC1394_VCR_COMMAND_WIND | AVC1394_VCR_OPERAND_WIND_FAST_FORWARD);

	}
}


void avc1394_vcr_next(raw1394handle_t handle, nodeid_t node)
{
	if (avc1394_vcr_is_playing(handle, node)) {
		avc1394_send_command(handle, node, CTLVCR0
			| AVC1394_VCR_COMMAND_PLAY | AVC1394_VCR_OPERAND_PLAY_NEXT_FRAME);
	} 
}


void avc1394_vcr_previous(raw1394handle_t handle, nodeid_t node)
{
	if (avc1394_vcr_is_playing(handle, node)) {
		avc1394_send_command(handle, node, CTLVCR0
			| AVC1394_VCR_COMMAND_PLAY | AVC1394_VCR_OPERAND_PLAY_PREVIOUS_FRAME);
	} 
}


void avc1394_vcr_eject(raw1394handle_t handle, nodeid_t node)
{
	avc1394_send_command(handle, node, CTLVCR0
		| AVC1394_VCR_COMMAND_LOAD_MEDIUM | AVC1394_VCR_OPERAND_LOAD_MEDIUM_EJECT);
}


void avc1394_vcr_record(raw1394handle_t handle, nodeid_t node)
{
	avc1394_send_command(handle, node, CTLVCR0
		| AVC1394_VCR_COMMAND_RECORD | AVC1394_VCR_OPERAND_RECORD_RECORD);
}

quadlet_t avc1394_vcr_status(raw1394handle_t handle, nodeid_t node)
{
	return avc1394_transaction(handle, node,
			STATVCR0 | AVC1394_VCR_COMMAND_TRANSPORT_STATE
			| AVC1394_VCR_OPERAND_TRANSPORT_STATE, AVC1394_RETRY);

}

char *avc1394_vcr_decode_status(quadlet_t response)
{
    /*quadlet_t resp0 = AVC1394_MASK_RESPONSE_OPERAND(response, 0);
    quadlet_t resp1 = AVC1394_MASK_RESPONSE_OPERAND(response, 1);*/
    quadlet_t resp2 = AVC1394_MASK_RESPONSE_OPERAND(response, 2);
    quadlet_t resp3 = AVC1394_MASK_RESPONSE_OPERAND(response, 3);

    if (response == 0) {
        return "OK";
    } else if (resp2 == AVC1394_VCR_RESPONSE_TRANSPORT_STATE_LOAD_MEDIUM) {
        return("Loading Medium");
    } else if (resp2 == AVC1394_VCR_RESPONSE_TRANSPORT_STATE_RECORD) {
        return("Recording");
    } else if (resp2 == AVC1394_VCR_RESPONSE_TRANSPORT_STATE_PLAY) {
        if (resp3 >= AVC1394_VCR_OPERAND_PLAY_FAST_FORWARD_1
                && resp3 <= AVC1394_VCR_OPERAND_PLAY_FASTEST_FORWARD) {
            return("Playing Fast Forward");
        } else if (resp3 >= AVC1394_VCR_OPERAND_PLAY_FAST_REVERSE_1
                   && resp3 <= AVC1394_VCR_OPERAND_PLAY_FASTEST_REVERSE) {
            return("Playing Reverse");
        } else if (resp3 == AVC1394_VCR_OPERAND_PLAY_FORWARD_PAUSE) {
            return("Playing Paused");
        } else {
            return("Playing");
        }
    } else if (resp2 == AVC1394_VCR_RESPONSE_TRANSPORT_STATE_WIND) {
        if (resp3 == AVC1394_VCR_OPERAND_WIND_HIGH_SPEED_REWIND) {
            return("Winding backward at incredible speed");
        } else if (resp3 == AVC1394_VCR_OPERAND_WIND_STOP) {
            return("Winding stopped");
        } else if (resp3 == AVC1394_VCR_OPERAND_WIND_REWIND) {
            return("Winding reverse");
        } else if (resp3 == AVC1394_VCR_OPERAND_WIND_FAST_FORWARD) {
            return("Winding forward");
        } else {
            return("Winding");
        }
    } else {
        return("Unknown");
    }
}

