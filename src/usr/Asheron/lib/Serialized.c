/*
 * NAME:	deSerialize()
 * DESCRIPTION:	Deserialize a string, based on a format description.  Returns
 *		an array with results.
 */
static mixed *deSerialize(string serialized, string format, varargs int number)
{
    mixed *results;
    int len, n, offset, i, j, x;

    if (number == 0) {
	number = 1;	/* default to 1 struct */
    }
    len = strlen(format);

    results = allocate(number * len);
    n = offset = 0;

    for (i = 0; i < number; i++) {
	for (j = 0; j < len; j++) {
	    switch (format[j]) {
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

	    default:
		error("Unknown format '" + format[j .. j] + "'");
		break;
	    }
	    results[n++] = x;
	}
    }

    return results;
}
