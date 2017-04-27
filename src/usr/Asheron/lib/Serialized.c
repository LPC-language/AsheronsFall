# define FLT_BIAS	127
# define DBL_BIAS	1023

/*
 * Serialize a number of arguments, given a format string.  Return a string
 * with the serialized data.
 */
static string serialize(string format, varargs mixed args...)
{
    int len, i, n, offset, num, exponent;
    string *result, str;
    float mantissa, bits;

    len = strlen(format);
    result = allocate(len);
    n = offset = 0;
    for (i = 0; i < len; i++) {
	switch (format[i]) {
	case '0':
	    /*
	     * zero padding
	     */
	    str = "\0";
	    offset++;
	    break;

	case 'i':
	    /*
	     * 4 byte integer, little endian
	     */
	    num = args[n++];
	    str = "\0\0\0\0";
	    str[0] = num;
	    str[1] = num >> 8;
	    str[2] = num >> 16;
	    str[3] = num >> 24;
	    offset += 4;
	    break;

	case 'p':
	    /*
	     * packed 31 bit integer, not quite little endian
	     */
	    num = args[n++];
	    if (num <= 0x7fff) {
		str = "\0\0";
		str[0] = num;
		str[1] = num >> 8;
		offset += 2;
	    } else {
		str = "\0\0\0\0";
		str[0] = num >> 16;
		str[1] = (num >> 24) + 0x80;
		str[2] = num;
		str[3] = num >> 8;
		offset += 4;
	    }
	    break;

	case 's':
	    /*
	     * 2 byte integer, little endian
	     */
	    num = args[n++];
	    str = "\0\0";
	    str[0] = num;
	    str[1] = num >> 8;
	    offset += 2;
	    break;

	case 'c':
	    /*
	     * 1 byte integer
	     */
	    str = "\0";
	    str[0] = args[n++];
	    offset++;
	    break;

	case 't':
	    /*
	     * variable length text
	     */
	    str = args[n++];
	    num = strlen(str);
	    str = "\0\0" + str;
	    str[0] = num;
	    str[1] = num >> 8;
	    offset += num + 2;
	    if (offset & 3) {
		str += "\0\0\0"[.. 3 - (offset & 3)];
		offset = (offset + 3) & ~3;
	    }
	    break;

	case 'f':
	    /*
	     * 4 byte IEEE float, little endian
	     */
	    str = "\0\0\0\0";
	    offset += 4;
	    ({ mantissa, exponent }) = frexp(args[n++]);
	    if (mantissa != 0.0) {
		exponent += FLT_BIAS - 1;
		if (exponent > 0) {
		    num = 24;
		} else {
		    num = exponent + 23;
		    if (num <= 0) {
			break;
		    }
		    exponent = 0;	/* denormalized */
		}
		if (mantissa < 0.0) {
		    /* negative */
		    mantissa = -mantissa;
		    str[3] = 0x80 + (exponent >> 1);
		} else {
		    str[3] = exponent >> 1;
		}
		num = (int) floor(ldexp(mantissa, num));
		str[2] = ((num >> 16) & 0x7f) + (exponent << 7);
		str[1] = num >> 8;
		str[0] = num;
	    }
	    break;

	case 'd':
	    /*
	     * 8 byte IEEE double, little endian
	     */
	    str = "\0\0\0\0\0\0\0\0";
	    offset += 8;
	    ({ mantissa, exponent }) = frexp(args[n++]);
	    if (mantissa != 0.0) {
		exponent += DBL_BIAS - 1;
		if (exponent > 0) {
		    num = 29;
		} else {
		    num = exponent + 28;
		    if (num <= 0) {
			break;
		    }
		    exponent = 0;	/* denormalized */
		}
		if (mantissa < 0.0) {
		    /* negative */
		    mantissa = -mantissa;
		    str[7] = 0x80 + (exponent >> 4);
		} else {
		    str[7] = exponent >> 4;
		}
		mantissa = ldexp(mantissa, num);
		bits = floor(mantissa);
		num = (int) bits;
		str[6] = ((num >> 24) & 0x0f) + (exponent << 4);
		str[5] = num >> 16;
		str[4] = num >> 8;
		str[3] = num;
		num = (int) floor(ldexp(mantissa - bits, 24));
		str[2] = num >> 16;
		str[1] = num >> 8;
		str[0] = num;
	    }
	    break;

	default:
	    error("Unknown format '" + format[i .. i] + "'");
	}
	result[i] = str;
    }

    return implode(result, "");
}

/*
 * Deserialize a string, based on a format description.  Returns an array
 * containing the remaining unserialized input, and the results.
 */
static mixed *deSerialize(string serialized, string format, varargs int number)
{
    mixed *results;
    int len, n, offset, i, j;
    int exponent, mantissa24, mantissa28;
    mixed x;

    if (number == 0) {
	number = 1;	/* default to 1 struct */
    }
    len = strlen(format);

    results = allocate(1 + number * len);
    n = offset = 0;

    for (i = 0; i < number; i++) {
	for (j = 0; j < len; j++) {
	    switch (format[j]) {
	    case '0':
		/*
		 * padding
		 */
		offset++;
		continue;

	    case 'i':
		/*
		 * 4 byte integer, little endian
		 */
		x = serialized[offset] +
		    (serialized[offset + 1] << 8) +
		    (serialized[offset + 2] << 16) +
		    (serialized[offset + 3] << 24);
		offset += 4;
		break;

	    case 'p':
		/*
		 * packed 31 bit integer, not quite little endian
		 */
		x = serialized[offset] +
		    (serialized[offset + 1] << 8);
		if (x <= 0x7fff) {
		    offset += 2;
		} else {
		    x = ((x & 0x7fff) << 16) +
			serialized[offset + 2] +
			(serialized[offset + 3] << 8);
		    offset += 4;
		}
		break;

	    case 's':
		/*
		 * 2 byte integer, little endian
		 */
		x = serialized[offset] +
		    (serialized[offset + 1] << 8);
		offset += 2;
		break;

	    case 'c':
		/*
		 * 1 byte
		 */
		x = serialized[offset];
		offset++;
		break;

	    case 't':
		/*
		 * variable length text
		 */
		x = serialized[offset] +
		    (serialized[offset + 1] << 8);
		offset += 2 + x;
		x = serialized[offset - x .. offset - 1];
		offset = (offset + 3) & ~3;
		break;

	    case 'f':
		/*
		 * 4 byte IEEE float, little endian
		 */
		exponent = ((serialized[offset + 3] & 0x7f) << 1) +
			   (serialized[offset + 2] >> 7);
		mantissa24 = ((serialized[offset + 2] & 0x7f) << 16) +
			     (serialized[offset + 1] << 8) +
			     serialized[offset];
		if (exponent == 0) {
		    if (mantissa24 == 0) {
			x = 0.0;
			break;
		    }
		    exponent = 1;		/* denormalized */
		} else {
		    mantissa24 += 0x800000;	/* implied high bit */
		}
		x = ldexp((float) mantissa24, exponent - FLT_BIAS - 23);
		if (serialized[offset + 3] & 0x80) {
		    x = -x;	/* negative */
		}
		offset += 4;
		break;

	    case 'd':
		/*
		 * 8 byte IEEE float, little endian
		 */
		exponent = ((serialized[offset + 7] & 0x7f) << 4) +
			   (serialized[offset + 6] >> 4);
		mantissa28 = ((serialized[offset + 6] & 0x0f) << 24) +
			     (serialized[offset + 5] << 16) +
			     (serialized[offset + 4] << 8) +
			     serialized[offset + 3];
		mantissa24 = (serialized[offset + 2] << 16) +
			     (serialized[offset + 1] << 8) +
			     serialized[offset];
		if (exponent == 0) {
		    if (mantissa24 | mantissa28 == 0) {
			x = 0.0;
			break;
		    }
		    exponent = 1;		/* denormalized */
		} else {
		    mantissa28 += 0x10000000;	/* implied high bit */
		}
		x = ldexp((float) mantissa28, exponent - DBL_BIAS - 28) +
		    ldexp((float) mantissa24, exponent - DBL_BIAS - 52);
		if (serialized[offset + 7] & 0x80) {
		    x = -x;	/* negative */
		}
		break;

	    default:
		error("Unknown format '" + format[j .. j] + "'");
	    }
	    results[++n] = x;
	}
    }

    results[0] = serialized[offset ..];
    return results;
}
