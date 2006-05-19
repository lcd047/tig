 * Browse changes in a git repository. Additionally, tig(1) can also act
 * as a pager for output of various git commands.
 *
 * When browsing repositories, tig(1) uses the underlying git commands
 * to present the user with various views, such as summarized commit log
 * and showing the commit with the log message, diffstat, and the diff.
 *
 * Using tig(1) as a pager, it will display input from stdin and try
 * to colorize it.
#define VERSION	"tig-0.3"
#define TIG_HELP	"(d)iff, (l)og, (m)ain, (q)uit, (h)elp"

#define TABSIZE		8


	REQ_SCREEN_RESIZE,
	REQ_MOVE_UP_ENTER,
	REQ_MOVE_DOWN_ENTER,
struct ref {
	char *name;		/* Ref name; tag or head names are shortened. */
	char id[41];		/* Commit SHA1 ID */
	unsigned int tag:1;	/* Is it a tag? */
	unsigned int next:1;	/* For ref lists: are there more refs? */
};

	struct ref **refs;	/* Repository references; tags & branch heads. */

string_ncopy(char *dst, const char *src, int dstlen)

#define BUFPUT(x) do { if (bufsize < SIZEOF_CMD) buf[bufsize++] = (x); } while (0)
/* Option and state variables. */
static bool opt_line_number	= FALSE;
static int opt_tab_size		= TABSIZE;
		/**
		 * -t[NSPACES], --tab-size[=NSPACES]::
		 *	Set the number of spaces tabs should be expanded to.
		 **/
		if (!strncmp(opt, "-t", 2) ||
		    !strncmp(opt, "--tab-size", 10)) {
			char *num = opt;

			if (opt[1] == 't') {
				num = opt + 2;

			} else if (opt[STRING_SIZE("--tab-size")] == '=') {
				num = opt + STRING_SIZE("--tab-size=");
			}

			if (isdigit(*num))
				opt_tab_size = MIN(atoi(num), TABSIZE);
			continue;
		}

		 *	End of tig(1) options. Useful when specifying command
		 *	options for the main view. Example:
		 * log [git log options]::
		 * diff [git diff options]::
		 * show [git show options]::
		/**
		 * [git log options]::
		 *	tig(1) will stop the option parsing when the first
		 *	command line parameter not starting with "-" is
		 *	encountered. All options including this one will be
		 *	passed to git log when loading the main view.
		 *	This makes it possible to say:
		 **/
		 * before they are passed to the shell.
		 * NOTE: If you specify options for the main view, you should
		 * not use the `--pretty` option as this option will be set
		 * automatically to the format expected by the main view.
		 *
		 * See the <<refspec, "Specifying revisions">> section below
		 * for an introduction to revision options supported by the git
		 * commands. For details on specific git command options, refer
		 * to the man page of the command in question.
LINE(DIFF_OLDMODE, "old file mode ",	COLOR_YELLOW,	COLOR_DEFAULT,	0), \
LINE(DIFF_NEWMODE, "new file mode ",	COLOR_YELLOW,	COLOR_DEFAULT,	0), \
LINE(MAIN_DELIM,   "",			COLOR_MAGENTA,	COLOR_DEFAULT,	0), \
LINE(MAIN_TAG,     "",			COLOR_MAGENTA,	COLOR_DEFAULT,	A_BOLD), \
LINE(MAIN_REF,     "",			COLOR_CYAN,	COLOR_DEFAULT,	A_BOLD),
	const char *line;	/* The start of line to match. */
 * Several options related to the interface with git can be configured
 * via environment options.
 *
 * Repository references
 * ~~~~~~~~~~~~~~~~~~~~~
 * Commits that are referenced by tags and branch heads will be marked
 * by the reference name surrounded by '[' and ']':
 *
 *	2006-03-26 19:42 Petr Baudis         | [cogito-0.17.1] Cogito 0.17.1
 *
 * If you want to filter out certain directories under `.git/refs/`, say
 * `tmp` you can do it by setting the following variable:
 *
 *	$ TIG_LS_REMOTE="git ls-remote . | sed /\/tmp\//d" tig
 *
 * Or set the variable permanently in your environment.
 *
 * TIG_LS_REMOTE::
 *	Set command for retrieving all repository references. The command
 *	should output data in the same format as git-ls-remote(1).
 **/

#define TIG_LS_REMOTE \
	"git ls-remote . 2>/dev/null"

/**
 * [[view-commands]]
 * View commands
 * ~~~~~~~~~~~~~
 * If for example you prefer commits in the main view to be sorted by date
 * and only show 500 commits, use:
 *	The command used for the log view. If you prefer to have both
 *	author and committer shown in the log view be sure to pass
 *	`--pretty=fuller` to git log.
/* ... silently ignore that the following are also exported. */
	"man tig 2>/dev/null"
/**
 * The viewer
 * ----------
 * The display consists of a status window on the last line of the screen and
 * one or more views. The default is to only show one view at the time but it
 * is possible to split both the main and log view to also show the commit
 * diff.
 *
 * If you are in the log view and press 'Enter' when the current line is a
 * commit line, such as:
 *
 *	commit 4d55caff4cc89335192f3e566004b4ceef572521
 *
 * You will split the view so that the log view is displayed in the top window
 * and the diff view in the bottom window. You can switch between the two
 * views by pressing 'Tab'. To maximize the log view again, simply press 'l'.
 **/

struct view;

/* The display array of active views and the index of the current view. */
static struct view *display[2];
static unsigned int current_view;

#define foreach_view(view, i) \
	for (i = 0; i < ARRAY_SIZE(display) && (view = display[i]); i++)


/**
 * Current head and commit ID
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~
 * The viewer keeps track of both what head and commit ID you are currently
 * viewing. The commit ID will follow the cursor line and change everytime time
 * you highlight a different commit. Whenever you reopen the diff view it
 * will be reloaded, if the commit ID changed.
 *
 * The head ID is used when opening the main and log view to indicate from
 * what revision to show history.
 **/

static char ref_commit[SIZEOF_REF]	= "HEAD";
static char ref_head[SIZEOF_REF]	= "HEAD";

	const char *cmd_fmt;	/* Default command line format */
	const char *cmd_env;	/* Command line set via environment */
	const char *id;		/* Points to either of ref_{head,commit} */
		/* What type of content being displayed. Used in the
		 * title bar. */
		const char *type;
/**
 * Views
 * ~~~~~
 * tig(1) presents various 'views' of a repository. Each view is based on output
 * from an external command, most often 'git log', 'git diff', or 'git show'.
 *
 * The main view::
 *	Is the default view, and it shows a one line summary of each commit
 *	in the chosen list of revisions. The summary includes commit date,
 *	author, and the first line of the log message. Additionally, any
 *	repository references, such as tags, will be shown.
 *
 * The log view::
 *	Presents a more rich view of the revision log showing the whole log
 *	message and the diffstat.
 *
 * The diff view::
 *	Shows either the diff of the current working tree, that is, what
 *	has changed since the last commit, or the commit diff complete
 *	with log message, diffstat and diff.
 *
 * The pager view::
 *	Is used for displaying both input from stdin and output from git
 *	commands entered in the internal prompt.
 *
 * The help view::
 *	Displays the information from the tig(1) man page. For the help view
 *	to work you need to have the tig(1) man page installed.
 **/

	VIEW_(HELP,  "help",  &pager_ops, "static",   sizeof(char)),

/**
 * Title windows
 * ~~~~~~~~~~~~~
 * Each view has a title window which shows the name of the view, current
 * commit ID if available, and where the view is positioned:
 *
 *	[main] c622eefaa485995320bc743431bae0d497b1d875 - commit 1 of 61 (1%)
 *
 * By default, the title of the current view is highlighted using bold font.
 **/

static void
update_view_title(struct view *view)
{
	if (view == display[current_view])
		wbkgdset(view->title, get_line_attr(LINE_TITLE_FOCUS));
	else
		wbkgdset(view->title, get_line_attr(LINE_TITLE_BLUR));

	werase(view->title);
	wmove(view->title, 0, 0);

	if (*view->ref)
		wprintw(view->title, "[%s] %s", view->name, view->ref);
	else
		wprintw(view->title, "[%s]", view->name);

	if (view->lines) {
		wprintw(view->title, " - %s %d of %d (%d%%)",
			view->ops->type,
			view->lineno + 1,
			view->lines,
			(view->lineno + 1) * 100 / view->lines);
	}

	wrefresh(view->title);
}

		/* Keep the height of all view->win windows one larger than is
		 * required so that the cursor can wrap-around on the last line
		 * without scrolling the window. */
			view->win = newwin(view->height + 1, 0, offset, 0);
			wresize(view->win, view->height + 1, view->width);
redraw_display(void)
	struct view *view;
	int i;
	foreach_view (view, i) {
		redraw_view(view);
		update_view_title(view);

		if (view->lineno == view->offset + view->height) {
			/* Clear the hidden line so it doesn't show if the view
			 * is scrolled up. */
			wmove(view->win, view->height, 0);
			wclrtoeol(view->win);
		}
	case REQ_MOVE_UP_ENTER:
	case REQ_MOVE_DOWN_ENTER:
	const char *id = view->id;
		const char *format = view->cmd_env ? view->cmd_env : view->cmd_fmt;
		int linelen = strlen(line);
			const char *msg = TIG_HELP;

			if (view->lines == 0) {
				/* Slightly ugly, but abusing view->ref keeps
				 * the error message. */
				string_copy(view->ref, "No help available");
				msg = "The tig(1) manpage is not installed";
			}

			report("%s", msg);
			report("");
		if (!backgrounded)
			update_view_title(prev);
		if (view == VIEW(REQ_VIEW_HELP))
			report("%s", TIG_HELP);
		else
			report("");

	/* If the view is backgrounded the above calls to report()
	 * won't redraw the view title. */
	if (backgrounded)
		update_view_title(view);
	case REQ_MOVE_UP_ENTER:
	case REQ_MOVE_DOWN_ENTER:
		move_view(view, request);
		/* Fall-through */

		report("");
		redraw_display();
				report("Stopped loaded the %s view", view->name),
	case REQ_SCREEN_RESIZE:
		resize_display();
		/* Fall-through */
		redraw_display();
		report("Unknown key, press 'h' for help");
	wmove(view->win, lineno, 0);

		wchgat(view->win, -1, 0, type, NULL);
	if (opt_line_number || opt_tab_size < TABSIZE) {
		static char spaces[] = "                    ";
		int col_offset = 0, col = 0;
		if (opt_line_number) {
			unsigned long real_lineno = view->offset + lineno + 1;
			if (real_lineno == 1 ||
			    (real_lineno % opt_num_interval) == 0) {
				wprintw(view->win, "%.*d", view->digits, real_lineno);
			} else {
				waddnstr(view->win, spaces,
					 MIN(view->digits, STRING_SIZE(spaces)));
			}
			waddstr(view->win, ": ");
			col_offset = view->digits + 2;
		}

		while (line && col_offset + col < view->width) {
			int cols_max = view->width - col_offset - col;
			char *text = line;
			int cols;
				assert(sizeof(spaces) > TABSIZE);
				text = spaces;
				cols = opt_tab_size - (col % opt_tab_size);
				line = strchr(line, '\t');
				cols = line ? line - text : strlen(text);

			waddnstr(view->win, text, MIN(cols, cols_max));
			col += cols;
		int col = 0, pos = 0;
		for (; pos < linelen && col < view->width; pos++, col++)
			if (line[pos] == '\t')
				col += TABSIZE - (col % TABSIZE) - 1;

		waddnstr(view->win, line, pos);
	}
	/* Compress empty lines in the help view. */
	if (view == VIEW(REQ_VIEW_DIFF)) {
		scroll_view(view, REQ_SCROLL_LINE_DOWN);
		return TRUE;
	}

		if (view == VIEW(REQ_VIEW_LOG))
			open_view(view, REQ_VIEW_DIFF, OPEN_SPLIT | OPEN_BACKGROUNDED);
		else
			open_view(view, REQ_VIEW_DIFF, OPEN_DEFAULT);
	"line",
static struct ref **get_refs(char *id);

	int col = 0;
	wmove(view->win, lineno, col);

		wattrset(view->win, get_line_attr(type));
		wchgat(view->win, -1, 0, type, NULL);

		wattrset(view->win, get_line_attr(LINE_MAIN_DATE));
	col += DATE_COLS;
	wmove(view->win, lineno, col);
	if (type != LINE_CURSOR)
		wattrset(view->win, get_line_attr(LINE_MAIN_AUTHOR));
		if (type != LINE_CURSOR)
			wattrset(view->win, get_line_attr(LINE_MAIN_DELIM));
	col += 20;
	if (type != LINE_CURSOR)
		wattrset(view->win, A_NORMAL);

	mvwaddch(view->win, lineno, col, ACS_LTEE);
	wmove(view->win, lineno, col + 2);
	col += 2;

	if (commit->refs) {
		size_t i = 0;

		do {
			if (type == LINE_CURSOR)
				;
			else if (commit->refs[i]->tag)
				wattrset(view->win, get_line_attr(LINE_MAIN_TAG));
			else
				wattrset(view->win, get_line_attr(LINE_MAIN_REF));
			waddstr(view->win, "[");
			waddstr(view->win, commit->refs[i]->name);
			waddstr(view->win, "]");
			if (type != LINE_CURSOR)
				wattrset(view->win, A_NORMAL);
			waddstr(view->win, " ");
			col += strlen(commit->refs[i]->name) + STRING_SIZE("[] ");
		} while (commit->refs[i++]->next);
	}

	if (type != LINE_CURSOR)
		wattrset(view->win, get_line_attr(type));

	{
		int titlelen = strlen(commit->title);

		if (col + titlelen > view->width)
			titlelen = view->width - col;

		waddnstr(view->win, commit->title, titlelen);
	}
		commit->refs = get_refs(commit->id);
	"commit",

/**
 * KEYS
 * ----
 * Below the default key bindings are shown.
 **/

struct keymap {
	int alias;
	int request;
};

static struct keymap keymap[] = {
	/**
	 * View switching
	 * ~~~~~~~~~~~~~~
	 * m::
	 *	Switch to main view.
	 * d::
	 *	Switch to diff view.
	 * l::
	 *	Switch to log view.
	 * p::
	 *	Switch to pager view.
	 * h::
	 *	Show man page.
	 * Enter::
	 *	This key is "context sensitive" depending on what view you are
	 *	currently in. When in log view on a commit line or in the main
	 *	view, split the view and show the commit diff. In the diff view
	 *	pressing Enter will simply scroll the view one line down.
	 * Tab::
	 *	Switch to next view.
	 **/
	{ 'm',		REQ_VIEW_MAIN },
	{ 'd',		REQ_VIEW_DIFF },
	{ 'l',		REQ_VIEW_LOG },
	{ 'p',		REQ_VIEW_PAGER },
	{ 'h',		REQ_VIEW_HELP },

	{ KEY_TAB,	REQ_VIEW_NEXT },
	{ KEY_RETURN,	REQ_ENTER },

	/**
	 * Cursor navigation
	 * ~~~~~~~~~~~~~~~~~
	 * Up::
	 *	Move cursor one line up.
	 * Down::
	 *	Move cursor one line down.
	 * k::
	 *	Move cursor one line up and enter. When used in the main view
	 *	this will always show the diff of the current commit in the
	 *	split diff view.
	 * j::
	 *	Move cursor one line down and enter.
	 * PgUp::
	 * b::
	 *	Move cursor one page up.
	 * PgDown::
	 * Space::
	 *	Move cursor one page down.
	 * Home::
	 *	Jump to first line.
	 * End::
	 *	Jump to last line.
	 **/
	{ KEY_UP,	REQ_MOVE_UP },
	{ KEY_DOWN,	REQ_MOVE_DOWN },
	{ 'k',		REQ_MOVE_UP_ENTER },
	{ 'j',		REQ_MOVE_DOWN_ENTER },
	{ KEY_HOME,	REQ_MOVE_FIRST_LINE },
	{ KEY_END,	REQ_MOVE_LAST_LINE },
	{ KEY_NPAGE,	REQ_MOVE_PAGE_DOWN },
	{ ' ',		REQ_MOVE_PAGE_DOWN },
	{ KEY_PPAGE,	REQ_MOVE_PAGE_UP },
	{ 'b',		REQ_MOVE_PAGE_UP },

	/**
	 * Scrolling
	 * ~~~~~~~~~
	 * Insert::
	 *	Scroll view one line up.
	 * Delete::
	 *	Scroll view one line down.
	 * w::
	 *	Scroll view one page up.
	 * s::
	 *	Scroll view one page down.
	 **/
	{ KEY_IC,	REQ_SCROLL_LINE_UP },
	{ KEY_DC,	REQ_SCROLL_LINE_DOWN },
	{ 'w',		REQ_SCROLL_PAGE_UP },
	{ 's',		REQ_SCROLL_PAGE_DOWN },

	/**
	 * Misc
	 * ~~~~
	 * q::
	 *	Quit
	 * r::
	 *	Redraw screen.
	 * z::
	 *	Stop all background loading. This can be useful if you use
	 *	tig(1) in a repository with a long history without limiting
	 *	the revision log.
	 * v::
	 *	Show version.
	 * n::
	 *	Toggle line numbers on/off.
	 * ':'::
	 *	Open prompt. This allows you to specify what git command
	 *	to run. Example:
	 *
	 *	:log -p
	 **/
	{ 'q',		REQ_QUIT },
	{ 'z',		REQ_STOP_LOADING },
	{ 'v',		REQ_SHOW_VERSION },
	{ 'r',		REQ_SCREEN_REDRAW },
	{ 'n',		REQ_TOGGLE_LINE_NUMBERS },
	{ ':',		REQ_PROMPT },

	/* wgetch() with nodelay() enabled returns ERR when there's no input. */
	{ ERR,		REQ_SCREEN_UPDATE },

	/* Use the ncurses SIGWINCH handler. */
	{ KEY_RESIZE,	REQ_SCREEN_RESIZE },
};

static enum request
get_request(int key)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(keymap); i++)
		if (keymap[i].alias == key)
			return keymap[i].request;

	return (enum request) key;
}


	static bool empty = TRUE;
	struct view *view = display[current_view];
	if (!empty || *msg) {
		va_list args;
		va_start(args, msg);
		werase(status_win);
		wmove(status_win, 0, 0);
		if (*msg) {
			vwprintw(status_win, msg, args);
			empty = FALSE;
		} else {
			empty = TRUE;
		}
		wrefresh(status_win);
		va_end(args);
	}

	update_view_title(view);

	/* Move the cursor to the right-most column of the cursor line.
	 *
	 * XXX: This could turn out to be a bit expensive, but it ensures that
	 * the cursor does not jump around. */
	if (view->lines) {
		wmove(view->win, view->lineno - view->offset, view->width - 1);
		wrefresh(view->win);
	}
	static unsigned int loading_views;
	if ((loading == FALSE && loading_views-- == 1) ||
	    (loading == TRUE  && loading_views++ == 0))

/*
 * Repository references
 */

static struct ref *refs;
static size_t refs_size;

static struct ref **
get_refs(char *id)
{
	struct ref **id_refs = NULL;
	size_t id_refs_size = 0;
	size_t i;

	for (i = 0; i < refs_size; i++) {
		struct ref **tmp;

		if (strcmp(id, refs[i].id))
			continue;

		tmp = realloc(id_refs, (id_refs_size + 1) * sizeof(*id_refs));
		if (!tmp) {
			if (id_refs)
				free(id_refs);
			return NULL;
		}

		id_refs = tmp;
		if (id_refs_size > 0)
			id_refs[id_refs_size - 1]->next = 1;
		id_refs[id_refs_size] = &refs[i];

		/* XXX: The properties of the commit chains ensures that we can
		 * safely modify the shared ref. The repo references will
		 * always be similar for the same id. */
		id_refs[id_refs_size]->next = 0;
		id_refs_size++;
	}

	return id_refs;
}

static int
load_refs(void)
{
	const char *cmd_env = getenv("TIG_LS_REMOTE");
	const char *cmd = cmd_env && *cmd_env ? cmd_env : TIG_LS_REMOTE;
	FILE *pipe = popen(cmd, "r");
	char buffer[BUFSIZ];
	char *line;

	if (!pipe)
		return ERR;

	while ((line = fgets(buffer, sizeof(buffer), pipe))) {
		char *name = strchr(line, '\t');
		struct ref *ref;
		int namelen;
		bool tag = FALSE;
		bool tag_commit = FALSE;

		if (!name)
			continue;

		*name++ = 0;
		namelen = strlen(name) - 1;

		/* Commits referenced by tags has "^{}" appended. */
		if (name[namelen - 1] == '}') {
			while (namelen > 0 && name[namelen] != '^')
				namelen--;
			if (namelen > 0)
				tag_commit = TRUE;
		}
		name[namelen] = 0;

		if (!strncmp(name, "refs/tags/", STRING_SIZE("refs/tags/"))) {
			if (!tag_commit)
				continue;
			name += STRING_SIZE("refs/tags/");
			tag = TRUE;

		} else if (!strncmp(name, "refs/heads/", STRING_SIZE("refs/heads/"))) {
			name += STRING_SIZE("refs/heads/");

		} else if (!strcmp(name, "HEAD")) {
			continue;
		}

		refs = realloc(refs, sizeof(*refs) * (refs_size + 1));
		if (!refs)
			return ERR;

		ref = &refs[refs_size++];
		ref->tag = tag;
		ref->name = strdup(name);
		if (!ref->name)
			return ERR;

		string_copy(ref->id, line);
	}

	if (ferror(pipe))
		return ERR;

	pclose(pipe);

	if (refs_size == 0)
		die("Not a git repository");

	return OK;
}

#if __GNUC__ >= 3
#define __NORETURN __attribute__((__noreturn__))
#else
#define __NORETURN
#endif

static void __NORETURN
static void __NORETURN
die(const char *err, ...)
	if (load_refs() == ERR)
		die("Failed to load refs.");

		/* Some low-level request handling. This keeps access to
		 * status_win restricted. */
		switch (request) {
		case REQ_PROMPT:
			break;

		case REQ_SCREEN_RESIZE:
		{
			int height, width;

			getmaxyx(stdscr, height, width);

			/* Resize the status view and let the view driver take
			 * care of resizing the displayed views. */
			wresize(status_win, 1, width);
			mvwin(status_win, height - 1, 0);
			wrefresh(status_win);
			break;
		}
		default:
			break;
 * [[refspec]]
 * Revision specification
 * ----------------------
 * This section describes various ways to specify what revisions to display
 * or otherwise limit the view to. tig(1) does not itself parse the described
 * revision options so refer to the relevant git man pages for futher
 * information. Relevant man pages besides git-log(1) are git-diff(1) and
 * git-rev-list(1).
 *
 * You can tune the interaction with git by making use of the options
 * explained in this section. For example, by configuring the environment
 * variables described in the  <<view-commands, "View commands">> section.
 *
 * Limit by path name
 * ~~~~~~~~~~~~~~~~~~
 * If you are interested only in those revisions that made changes to a
 * specific file (or even several files) list the files like this:
 *
 *	$ tig log Makefile README
 *
 * To avoid ambiguity with repository references such as tag name, be sure
 * to separate file names from other git options using "\--". So if you
 * have a file named 'master' it will clash with the reference named
 * 'master', and thus you will have to use:
 *
 *	$ tig log -- master
 *
 * NOTE: For the main view, avoiding ambiguity will in some cases require
 * you to specify two "\--" options. The first will make tig(1) stop
 * option processing and the latter will be passed to git log.
 *
 * Limit by date or number
 * ~~~~~~~~~~~~~~~~~~~~~~~
 * To speed up interaction with git, you can limit the amount of commits
 * to show both for the log and main view. Either limit by date using
 * e.g. `--since=1.month` or limit by the number of commits using `-n400`.
 *
 * If you are only interested in changed that happened between two dates
 * you can use:
 *
 *	$ tig -- --after="May 5th" --before="2006-05-16 15:44"
 *
 * NOTE: If you want to avoid having to quote dates containing spaces you
 * can use "." instead, e.g. `--after=May.5th`.
 *
 * Limiting by commit ranges
 * ~~~~~~~~~~~~~~~~~~~~~~~~~
 * Alternatively, commits can be limited to a specific range, such as
 * "all commits between 'tag-1.0' and 'tag-2.0'". For example:
 *
 *	$ tig log tag-1.0..tag-2.0
 *
 * This way of commit limiting makes it trivial to only browse the commits
 * which haven't been pushed to a remote branch. Assuming 'origin' is your
 * upstream remote branch, using:
 *
 *	$ tig log origin..HEAD
 *
 * will list what will be pushed to the remote branch. Optionally, the ending
 * 'HEAD' can be left out since it is implied.
 *
 * Limiting by reachability
 * ~~~~~~~~~~~~~~~~~~~~~~~~
 * Git interprets the range specifier "tag-1.0..tag-2.0" as
 * "all commits reachable from 'tag-2.0' but not from 'tag-1.0'".
 * Where reachability refers to what commits are ancestors (or part of the
 * history) of the branch or tagged revision in question.
 *
 * If you prefer to specify which commit to preview in this way use the
 * following:
 *
 *	$ tig log tag-2.0 ^tag-1.0
 *
 * You can think of '^' as a negation operator. Using this alternate syntax,
 * it is possible to further prune commits by specifying multiple branch
 * cut offs.
 *
 * Combining revisions specification
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Revisions options can to some degree be combined, which makes it possible
 * to say "show at most 20 commits from within the last month that changed
 * files under the Documentation/ directory."
 *
 *	$ tig -- --since=1.month -n20 -- Documentation/
 *
 * Examining all repository references
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * In some cases, it can be useful to query changes across all references
 * in a repository. An example is to ask "did any line of development in
 * this repository change a particular file within the last week". This
 * can be accomplished using:
 *
 *	$ tig -- --all --since=1.week -- Makefile
 *
 * BUGS
 * ----
 * Known bugs and problems:
 *
 * - If the screen width is very small the main view can draw
 *   outside the current view causing bad wrapping. Same goes
 *   for title and status windows.
 *
 * - Searching.
 * qgit(1): git repository browser written using c++/Qt,