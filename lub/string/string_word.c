/*
 * /lub/string/string_word.c
 */
#include <stddef.h>
#include <ctype.h>

#include "private.h"
#include "lub/types.h"

/*--------------------------------------------------------- */
const char *lub_string_nextword(const char *string,
	size_t *len, size_t *offset, size_t *quoted)
{
	const char *word;

	*quoted = 0;

	/* Find the start of a word (not including an opening quote) */
	while (*string && isspace(*string)) {
		string++;
		(*offset)++;
	}
	/* Is this the start of a quoted string ? */
	if (*string == '"') {
		*quoted = 1;
		string++;
	}
	word = string;
	*len = 0;

	/* Find the end of the word */
	while (*string) {
		if (*string == '\\') {
			string++;
			(*len)++;
			if (*string) {
				(*len)++;
				string++;
			}
			continue;
		}
		/* End of word */
		if (!*quoted && isspace(*string))
			break;
		if (*string == '"') {
			/* End of a quoted string */
			*quoted = 2;
			break;
		}
		(*len)++;
		string++;
	}

	return word;
}

/*--------------------------------------------------------- */
unsigned int lub_string_wordcount(const char *line)
{
	const char *word;
	unsigned int result = 0;
	size_t len = 0, offset = 0;
	size_t quoted;

	for (word = lub_string_nextword(line, &len, &offset, &quoted);
		*word || quoted;
		word = lub_string_nextword(word + len, &len, &offset, &quoted)) {
		/* account for the terminating quotation mark */
		len += quoted ? quoted - 1 : 0;
		result++;
	}

	return result;
}

/*--------------------------------------------------------- */
