# include <Iterator.h>

inherit Iterable;


private string dir;	/* initial directory */

/*
 * create file tree walker
 */
static void create(string path)
{
    dir = path;
}

/*
 * iterator initial state
 */
mixed iteratorStart(mixed from, mixed to)
{
    if (from != nil || to != nil) {
	error("Subrange not implemented");
    }
    return ({ dir, nil, ({ file_info(dir)[0] }), 0 });
}

/*
 * obtain the next file from the tree
 */
mixed *iteratorNext(mixed state)
{
    int size, index, *sizes, *times;
    string path, *names;
    mixed *objects;

    size = sizeof(state);
    if (size == 0) {
	return ({ state, nil });
    }

    ({ path, names, sizes, index }) = state[size - 4 ..];
    if (names) {
	path += "/" + names[index];
    }
    state[size - 1] = ++index;
    if (sizes[index - 1] == -2) {
	({ names, sizes, times, objects }) = get_dir(path + "/*");
	if (sizeof(sizes) != 0) {
	    return ({ state + ({ path, names, sizes, 0 }), path });
	}
    }

    while (index >= sizeof(state[size - 2])) {
	size -= 4;
	state = state[.. size - 1];
	if (size == 0) {
	    break;
	}
	index = state[size - 1];
    }

    return ({ state, path });
}

/*
 * no more files?
 */
int iteratorEnd(mixed state)
{
    return (sizeof(state) == 0);
}
