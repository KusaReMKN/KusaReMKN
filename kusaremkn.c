#include <err.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define DEVNULL	"/dev/null"
#define DATPATH	"./data/"
#define DATNAME	"%s.kusaremkn"

static void	discard(char *argv[]);
static void	usage(void);

int
main(int argc, char *argv[])
{
	int ch, count, i, num;
	char *lang, *p;
	FILE *fp;
	char boundary[BUFSIZ], buf[BUFSIZ], path[PATH_MAX+1];

	lang = "c";
	while ((ch = getopt(argc, argv, "l:")) != -1)
		switch (ch) {
		case 'l':
			p = strrchr(optarg, '/');
			lang = p == NULL ? optarg : p + 1;
			break;
		case '?':
		default:
			usage();
			/* NOTREACHED */
		}
	argc -= optind;
	argv += optind;
	discard(argv);

	srandom(time(NULL) + getpid());
	if ((random() & 0xFF) == 0xFF) {
quit:
		warnx("Mikan-chan is unmotivated.");
		raise(SIGQUIT);
	}

	snprintf(path, sizeof(path), DATPATH DATNAME, lang);
	if ((fp = fopen(path, "r")) == NULL)
		goto quit;
	fgets(buf, sizeof(buf), fp);
	sscanf(buf, "%d%s", &count, boundary);

	num = random() % count;
	i = 0;
	do {
		if (fgets(buf, sizeof(buf), fp) == NULL) {
			fclose(fp);
			goto quit;
		}
		if ((p = strchr(buf, '\n')) != NULL) {
			*p = '\0';
			if (strcmp(buf, boundary) == 0) {
				i++;
				continue;
			}
			*p = '\n';
		}
		if (i == num)
			fputs(buf, stdout);
	} while (i <= num);
	fclose(fp);

	return 0;
}

static void
discard(char *argv[])
{
	FILE *fnull, *fp;
	int i;
	char buf[BUFSIZ];

	if ((fnull = fopen(DEVNULL, "w")) == NULL)
		err(1, "%s: fopen", DEVNULL);
	for (i = 0; argv[i] != NULL || i == 0; i++) {
		if (argv[i] == NULL || strcmp(argv[i], "-") == 0)
			fp = stdin;
		else
			if ((fp = fopen(argv[i], "r")) == NULL) {
				warn("%s: fopen", argv[i]);
				continue;
			}
		while (fgets(buf, sizeof(buf), fp) != NULL)
			if (fputs(buf, fnull) == EOF) {
				warn("%s: fputs", DEVNULL);
				break;
			}
		if (argv[i] == NULL)
			break;
		fclose(fp);
	}
	fclose(fnull);
}

static void
usage(void)
{
	fprintf(stderr, "usage: KusaReMKN [-l language] [file ...]\n");
	exit(1);
}
