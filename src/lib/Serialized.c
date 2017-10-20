# define FLT_BIAS	127
# define DBL_BIAS	1023

/*
 * serialize a double precision floating point number
 */
private string serializeDouble(float flt)
{
    string str;
    float mantissa, bits;
    int exponent, num;

    str = "\0\0\0\0\0\0\0\0";
    if (flt != 0.0) {
	({ mantissa, exponent }) = frexp(flt);
	exponent += DBL_BIAS - 1;
	if (exponent > 0) {
	    num = 29;
	} else {
	    num = exponent + 28;
	    if (num <= 0) {
		return str;
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

    return str;
}

/*
 * serialize mantissa bits in a double precision floating point number
 */
private string serializeBits(string str, int bits, int exp, int space,
			     int offset)
{
    if (exp < space) {
	do {
	    str[offset++] |= bits << (space - exp);
	    space -= 8;
	} while (space > 32);
	if (exp < space) {
	    bits <<= space - exp;
	}
    }
    if (exp > space) {
	bits >>= exp - space;
    }

    while (bits != 0) {
	str[offset++] |= bits;
	bits >>= 8;
    }
    return str;
}

/*
 * serialize a "date" (an integer and a floating point fragment)
 */
private string serializeDate(int secs, float msecs)
{
    string str;
    float mantissa;
    int exponent, num;

    if (secs == 0) {
	return serializeDouble(msecs);
    } else if (msecs == 0.0) {
	return serializeDouble((float) secs);
    }

    str = "\0\0\0\0\0\0\0\0";
    ({ mantissa, exponent }) = frexp((float) secs);
    num = exponent - 1;
    exponent += DBL_BIAS - 1;
    str[7] = exponent >> 4;
    str[6] = exponent << 4;
    if (secs < 0) {
	secs = -secs;
	msecs = -msecs;
	str[7] |= 0x80;
    }
    str = serializeBits(str, secs ^ (1 << num), num, 36, 2);

    ({ mantissa, exponent }) = frexp(msecs);
    num = 52 - num + exponent;
    if (num > 0) {
	exponent = (num > 31) ? 31 : num;
	str = serializeBits(str, (int) floor(ldexp(mantissa, exponent)),
			    exponent, num, 0);
    }

    return str;
}

/*
 * Serialize a number of arguments, given a format string.  Return a string
 * with the serialized data.
 */
static string serialize(string format, varargs mixed args...)
{
    int len, i, n, offset, num, exponent;
    string *result, str;
    float mantissa;

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

	case 'I':
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

	case 'S':
	    /*
	     * packed 15 bit integer, big endian
	     */
	    num = args[n++];
	    if (num <= 0x7f) {
		str = "\0";
		str[0] = num;
	    } else {
		str = "\0\0";
		str[0] = (num >> 8) + 0x80;
		str[1] = num >> 8;
		offset += 2;
	    }
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
	     * variable length text, aligned
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

	case 'p':
	    /*
	     * variable length text, 8 bit length
	     */
	    str = args[n++];
	    num = strlen(str);
	    str = "\0" + str;
	    str[0] = num;
	    offset += num + 1;
	    break;

	case 'P':
	    /*
	     * variable length text, 16 bit length
	     */
	    str = args[n++];
	    num = strlen(str);
	    str = "\0\0" + str;
	    str[0] = num;
	    str[1] = num >> 8;
	    offset += num + 2;
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
	    str = serializeDouble(args[n++]);
	    offset += 8;
	    break;

	case 'D':	/* DD */
	    /*
	     * (int, float) as 8 byte IEEE double, little endian
	     */
	    result[i] = "";
	    if (format[++i] != 'D') {
		error("Unknown format 'D" + format[i .. i] + "'");
	    }
	    num = args[n++];
	    str = serializeDate(num, args[n++]);
	    offset += 8;
	    break;

	default:
	    error("Unknown format '" + format[i .. i] + "'");
	}
	result[i] = str;
    }

    return implode(result, "");
}

/*
 * add the padding needed to align to 4 bytes
 */
static string serializeAlign(string serialized)
{
    int size;

    size = strlen(serialized) & 3;
    return (size != 0) ? serialized + "\0\0\0\0"[size ..] : serialized;
}

/*
 * deserialize a double precision floating point number
 */
private float deSerializeDouble(string serialized, int offset)
{
    int exponent, mantissa24, mantissa28;
    float x;

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
	if ((mantissa24 | mantissa28) == 0) {
	    return 0.0;
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

    return x;
}

/*
 * extract mantissa bits from a double precision floating point number
 */
private int deSerializeBits(string serialized, int offset, int bits,
			    int bitOffset)
{
    int x, i, n;

    x = 0;
    i = (bitOffset >> 3) + 1;
    for (n = bits + ((bitOffset + 7) & ~0x7) - bitOffset; n >= 8; n -= 8) {
	x = (x << 8) | serialized[offset + --i];
    }
    if (n != 0) {
	x = (x << n) | (serialized[offset + --i] >> (8 - n));
    }
    return x & ~(0xffffffff << bits);
}

/*
 * deserialize a "date" from a double precision floating point number
 */
private mixed *deSerializeDate(string serialized, int offset)
{
    int exponent, bitOffset, x;
    float y;

    exponent = ((serialized[offset + 7] & 0x7f) << 4) +
	       (serialized[offset + 6] >> 4);
    if (exponent == 0) {
	return ({ 0, 0.0 });
    }
    exponent -= DBL_BIAS;
    if (exponent < 0) {
	return ({ 0, deSerializeDouble(serialized, offset) });
    }
    if (exponent > 30) {
	error("DD format number too large");
    }

    bitOffset = 52;
    x = (exponent != 0) ?
	 deSerializeBits(serialized, offset, exponent, bitOffset) |
							    (1 << exponent) :
	 1;
    bitOffset -= exponent;
    exponent = (bitOffset <= 31) ? bitOffset : 31;
    y = ldexp((float) deSerializeBits(serialized, offset, exponent, bitOffset),
	      -exponent);
    if (serialized[offset + 7] & 0x80) {
	x = -x;
	y = -y;
    }
    return ({ x, y });
}

/*
 * Deserialize a string, based on a format description.  Returns an array
 * containing the remaining unserialized input, and the results.
 */
static mixed *deSerialize(string serialized, int offset, string format,
			  varargs int number)
{
    mixed *results;
    int len, n, i, j;
    int exponent, mantissa;
    mixed x;

    if (number == 0) {
	number = 1;	/* default to 1 struct */
    }
    len = strlen(format);

    results = allocate(1 + number * len);
    n = 0;

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

	    case 'I':
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

	    case 'S':
		/*
		 * packed 15 bit integer, big endian
		 */
		x = serialized[offset];
		if (x <= 0x7f) {
		    offset++;
		} else {
		    x = ((x & 0x7f) << 8) +
			serialized[offset + 1];
		    offset += 2;
		}
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
		 * variable length text, aligned
		 */
		x = serialized[offset] +
		    (serialized[offset + 1] << 8);
		offset += 2 + x;
		x = serialized[offset - x .. offset - 1];
		offset = (offset + 3) & ~3;
		break;

	    case 'p':
		/*
		 * variable length text, 8 bit length
		 */
		x = serialized[offset];
		offset += 1 + x;
		x = serialized[offset - x .. offset - 1];
		break;

	    case 'P':
		/*
		 * variable length text, 16 bit length
		 */
		x = serialized[offset] +
		    (serialized[offset + 1] << 8);
		offset += 2 + x;
		x = serialized[offset - x .. offset - 1];
		break;

	    case 'f':
		/*
		 * 4 byte IEEE float, little endian
		 */
		exponent = ((serialized[offset + 3] & 0x7f) << 1) +
			   (serialized[offset + 2] >> 7);
		mantissa = ((serialized[offset + 2] & 0x7f) << 16) +
			   (serialized[offset + 1] << 8) +
			   serialized[offset];
		if (exponent == 0) {
		    if (mantissa == 0) {
			x = 0.0;
			offset += 4;
			break;
		    }
		    exponent = 1;		/* denormalized */
		} else {
		    mantissa += 0x800000;	/* implied high bit */
		}
		x = ldexp((float) mantissa, exponent - FLT_BIAS - 23);
		if (serialized[offset + 3] & 0x80) {
		    x = -x;	/* negative */
		}
		offset += 4;
		break;

	    case 'd':
		/*
		 * 8 byte IEEE float, little endian
		 */
		x = deSerializeDouble(serialized, offset);
		offset += 8;
		break;

	    case 'D':	/* DD */
		/*
		 * 8 byte IEEE float, little endian as (int, float)
		 */
		if (format[++j] != 'D') {
		    error("Unknown format 'D" + format[j .. j] + "'");
		}
		({ results[++n], x }) = deSerializeDate(serialized, offset);
		offset += 8;
		break;

	    default:
		error("Unknown format '" + format[j .. j] + "'");
	    }
	    results[++n] = x;
	}
    }

    results[0] = offset;
    return results;
}

/*
 * align to 4 bytes
 */
static int align(int size)
{
    return ((size + 3) & ~3);
}
