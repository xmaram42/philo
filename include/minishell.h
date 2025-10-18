/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Non-interactive/-c modes removed to align with subject */

/* includes */
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
# include "../libs/Libft/libft.h"

/* internal markers */
# define WILDMARK 31

typedef struct s_global_state
{
	int	signal_number;
	int	exit_status;
}	t_global_state;

/* internal global slots struct */
struct s_global_slots
{
	sig_atomic_t	signal_number;
	int				exit_status;
};

/* enums */
typedef enum e_message
{
	commandnotfound,
	filenameargumentrequired,
	invalidaliasname1,
	invalidaliasname2,
	notavalididentifier1,
	notavalididentifier2,
	notfound,
	numericargumentrequired,
	oldpwdnotset,
	syntaxerrornearunexpectedtoken1,
	syntaxerrornearunexpectedtoken2,
	toomanyarguments
}	t_message;

// Removed internationalization - not required for evaluation

/* structs */
typedef struct s_node
{
	bool	argmode;
	int		backup_stdin;
	int		backup_stdout;
	int		child_die;
	char	*cmd;
	int		echo_skip;
	bool	escape_skip;
	int		exit_flag;
	int		fds[2];
	int		i;
	bool	last;
	int		line_nbr;
	char	**ori_args;
	int		parent_die;
	char	*path;
	char	*path_fallback;
	int		pipe_flag;
	int		pipe_idx;
	char	*pwd;
	int		quota_idx_j;
	int		quota_pipe_cnt;
	int		quota_pipe_idx_arr[10];
	int		redir_fd;
	int		redir_fds[2];
	int		redir_flag;
	int		redir_idx;
	bool	semicolon_sequence;
	int		redir_stop;
	int		right_flag;
	bool	syntax_flag;
	/* flag: last heredoc ended due to EOF before delimiter */
	bool	heredoc_unterminated;
	/* flag: last heredoc consumed at least one line of input */
	bool	heredoc_swallowed_input;
}	t_node;

typedef struct s_prompt_data
{
	char	**fmt;
	char	*new_fmt;
	int		j;
	char	*user;
	char	*pwd;
}	t_prompt_data;

/* global state access */
int				get_signal_number(void);
void			clear_signal_number(void);
void			set_signal_number(int sig);
int				get_exit_status(void);
void			set_exit_status(int status);
/* internal single-slot exit-status accessor (implemented in global.c) */
int				_ms_exit_status(int op, int value);
/* non-interactive input marker (binary/non-text line seen) */
bool			get_nontext_input(void);
void			set_nontext_input(bool v);
void			clear_nontext_input(void);

/* quote processing state */
typedef struct s_quote_state
{
	bool	in_single;
	bool	in_double;
	int		j;
}	t_quote_state;

/* bonus hooks */
# ifdef BUILD_BONUS

char			**split_operators(char *s, char **envp, t_node *n);
char			**subshell(char *str, char **envp, t_node *node);
bool			check_wildcard_redirections(char **args);
char			**expand_wildcard_if_bonus(char **a, char **e, t_node *n);
# else
/* mandatory build: declare only what is actually used as no-ops */
char			**subshell(char *str, char **envp, t_node *node);
# endif

# ifdef BUILD_BONUS

bool			is_redir_token(char *s);
size_t			count_expanded_size(char **args);
# endif

/* cmds */
void			checkdot(char **args, char **envp, t_node *node);
void			chkdir(char **args, char **envp, bool end, t_node *node);
void			handle_env_i_option(char **args, char **envs, t_node *node);
/* alias features removed (not part of mandatory/bonus) */
char			**cmd_cd(char **args, char **envp, t_node *node);
char			*expand_tilde(char *path, char **envp);
bool			check_arg_count_with_slash(char **args, char **envp,
					int offset);
bool			check_arg_count_without_slash(char **args, char **envp,
					int offset);
bool			check_argument_count(char **args, char **envp, int offset);
bool			handle_home_cd(char **args, char **envp, int offset);
bool			handle_oldpwd_cd(char **args, char **envp, t_node *node,
					int offset);
bool			handle_chdir_error(char **args, int offset);
bool			checks2(char **args, char **envp, t_node *node,
					int offset);
bool			checks(char **args, char **envp, t_node *node,
					int offset);
bool			validate_cd_args(char **args, int offset);
char			**execute_cd(char **args, char **envp, t_node *node,
					int offset);
void			handle_cd_arguments(char **args, char **envp, t_node *node,
					int offset);
bool			update_pwd(char *path, t_node *node);
void			cmd_echo(char **args, t_node *node);
char			**cmd_env(char **args, char **envs, t_node *node);
char			**cmd_exec(char **args, char **envp, t_node *node);
void			cmd_exit(char **args, char **envp, t_node *node);
void			handle_exit_message(void);
void			handle_numeric_error(char *arg);
void			handle_too_many_args(void);
bool			handle_exit_with_args(char **args);
bool			exit_will_terminate(char **args);
void			cleanup_child_and_exit(char **args, char **envp, t_node *node);
void			cleanup_and_exit(char **args, char **envp, t_node *node);
void			handle_eof_exit(char **envp, t_node *node);
char			**cmd_export(char **args, char **envp, t_node *node);
char			**export_print(char **envp);
void			print_escaped_value(char *value);
bool			is_valid_identifier_start(char c);
bool			is_valid_identifier_char(char c);
void			print_invalid_identifier_error(char *arg);
bool			process_export_arg(char *arg, char ***envp, t_node *node);
void			handle_export_option_error(char **args);
void			process_export_args(char **args, char ***envp, t_node *node);
void			handle_path_update(char *arg, t_node *node);
char			**handle_env_update(char *arg, char **envp, char *name, int j);
bool			validate_export_identifier(char *arg);
void			cmd_pwd(t_node *node);
/* history built-in removed (not implemented) */
char			**cmd_unset(char **args, char **envp, t_node *node);
bool			is_valid_identifier(char *str);
int				find_envkey(char *str, char *envp);
char			**delete_env(char *str, char **envp, t_node *node, int *flag);
void			handle_unset_option_error(char **args);
void			process_unset_args(char **args, char **envp, t_node *node,
					int *flag);
bool			is_valid_identifier_start_unset(char c);
bool			is_valid_identifier_char_unset(char c);

/* exec helpers (norm-friendly) */
void			post_wait_set_status(int pid, int background);
bool			exec_check_loop(char **paths, char **args);
bool			exec_check(char **args, char **envp, t_node *node);
void			exec_error(char **args, char **envp, char **paths,
					t_node *node);
void			exec_proc(char **args, char **envp, t_node *node);
char			**exec_pipe(char *path, char **args, char **envp,
					t_node *node);
void			exec_nopath(t_node *node, char **args, char **envp,
					char **paths);
void			exec_proc_loop(char **paths, char **args, char **envp,
					t_node *node);
void			exec_proc_loop2(char **paths, char **args, char **envp,
					t_node *node);
char			*build_candidate(char *dir, char *cmd);
bool			is_builtin_command(char **args);
bool			is_executable(char *path);
bool			is_directory(char *path);
long long		ft_atoll(const char *str);
bool			ft_isalldigit(char *str);
char			*ft_getenv(const char *name, char **envp);
int				ft_setenv_var(const char *name, const char *value,
					int overwrite);
char			**ft_setenv_envp(const char *name, const char *value,
					char **envp);
char			**ensure_oldpwd_export(char **envp);
/* removed unused helpers newpwd/printenv/curdir/get_curdir */

/* main */
// Removed internationalization - not required for evaluation
char			*get_line(char *str);
char			*read_line_non_tty(void);
int				append_line(char **result, char *line);
char			*get_continuation_line(char *prompt);
int				process_read_line(char **result, char **cur_prompt, char *orig);
char			**get_file(int fd);
/* Non-interactive input helpers removed */
// Removed internationalization - not required for evaluation
// Removed run_commands - not required for evaluation
void			set_signal(void);
void			set_termios(void);
void			restore_termios(void);
char			**setpwd(t_node *node, char **envp);
char			**shlvl_mod(int mod, char **envp);

# ifdef BUILD_BONUS
/* Bonus-specific APIs are declared once below in the parser section. */
# endif

char			**shlvl_plus_plus(char **envp);
char			**strarradd(char **strs, char *str);
char			**strarradd_take(char **arr, char *owned);
char			**strarrdup(char **strs);
void			strarrfree(char **strs);

# ifdef DEBUG_MEMORY

/* Debug functions removed - not used in codebase */
# endif

size_t			strarrlen(char **strs);
char			**process_command(char *line, char **envp, t_node *n);
char			**check_standalone_operators(
					char *line, char **envp, t_node *n);
char			**get_prompt(char **envp, t_node *n);
/* process_command helpers */
bool			is_blank(const char *s);
char			**dispatch_line(char *hashed, char **envp, t_node *n);
int				check_operators_at_depth_zero(const char *s, int i,
					int *has_andand);
int				scan_for_oror(const char *s, t_node *n, int *has_andand);
char			**handle_oror_error(char *left, char *right, char *hashed,
					char **envp);
char			**split_operators(char *s, char **envp, t_node *n);

/* parser */
struct s_redir_copy_state
{
	int		i;
	int		j;
};
char			*return_marked_unchanged(char *str);
void			strip_wildmarks_inplace(char **args);
bool			error_message(const char *token, char **envp, t_node *node);
char			**escape_split(char *s, char *charset, t_node *node);
char			*expand_alias(char *str, t_node *node);
char			*get_command(char *str, int i, int offset, t_node *node);
char			*expand_envvar(char *str, char **envp, t_node *node);
void			process_envvar(char **str, char **envp, t_node *node, int *i);
void			insert_int(char *str, int *i);
char			*expand_prompt(char *fmt, char **envp, t_node *node);
void			expand_wildcard_loop(char **args, char **newargs, char **envp,
					t_node *node);
void			failcheck(char **files, int *i, char **newargs, char **args);
bool			wildcard_loop(char **files, int *i, char **newargs,
					char **args);
void			wildcard_handler(char **args, char **newargs, int *i,
					t_node *node);
char			*add_spaces_around_ampersand(char *str, t_node *node);
char			*add_spaces_around_redirections(char *str, t_node *node);
char			*add_spaces_around_logical_operators(char *str, t_node *node);
int				need_space_before_logical(char *s, int i);
int				need_space_after_logical(char *s, int i);
void			handle_oror_operator(char *str, char *result, int *i, int *j);
void			handle_andand_operator(char *str, char *result, int *i, int *j);
int				copy_double_left(char *str, char *result, int i, int *j);
int				copy_double_right(char *str, char *result, int i, int *j);
int				copy_single_redir(char *str, char *result, int i, int *j);
char			**find_command(char **args, char **envp, t_node *node);
char			**expand_wildcard(char **args, char **envp, t_node *node);
char			*expand_wildcard_redir(char *pattern, t_node *node);
char			**get_file_list(bool hidden);
int				get_arg_num(char **args, t_node *node);
char			**load_lst(struct dirent *dr, DIR *dir, bool hidden);
int				match_loop(char **split, char **files, int i);
void			match(char *str, char **split, char **files, int i);
void			get_length(char *str, char **envp, int *i, t_node *node);
char			*hash_handler(char *str, t_node *node);
bool			in_heredoc(char *str, int i);
bool			isdlr(char *str);
bool			isdrr(char *str);
bool			islr(char *str);
bool			islrr(char *str);
bool			isp(char *str);
bool			istr(char *str);
bool			isrr(char *str);
bool			istlr(char *str);
bool			isdp(char *str);
bool			isda(char *str);
bool			is_open_paren(char *str);
bool			is_close_paren(char *str);
bool			is_open_brace(char *str);
bool			is_close_brace(char *str);
char			**parser(char *str, char **envp, t_node *node);
int				quote_check(char const *s, int i, t_node *node);
char			**rm_quotes(char **args, t_node *node);
char			**rm_quotes_wildcards(char **args, t_node *node);
bool			syntax_check(char **args, char **envp, t_node *node);
void			tilde_handler(char **args, int *i, char **envp);
char			**apply_wildcard_phase(char **args, char **envp, t_node *node,
					char *orig);

/* syntax helpers */
const char		*check_leading_operators(char **args);
const char		*check_consecutive_ops(char **args, int i);
const char		*check_invalid_operator_sequences(char **args, int i);
const char		*check_triple_redir_split(char **args, int i);
const char		*check_final_token(char **args, int i);
bool			check_leading_operators_syntax(char **a);
bool			check_consecutive_operators_syntax(char **a);
bool			check_pipe_redir_combination(char **a, int i);
bool			check_trailing_operators_syntax(char **a);
const char		*get_error_token(char **args);
void			report_syntax_error(char c, t_node *node);
bool			c(char **args, int i, bool (*f1)(char *), bool (*f2)(char *));
void			handle_syntax_error(char **envp, t_node *node);
/* check_invalid_operator_sequences_in_string removed - not defined */
/* handle_redirect_ampersand removed - not defined */
char			**dispatch_builtin(char **args, char **envp, t_node *node);

/* pipe */
int				prepare_redirections(char **args, char **envp, t_node *node);
int				maybe_setup_pipe(t_node *node);
void			backup_restor(t_node *node);
char			**cloturn(int backup_stdout, int backup_stdin, char **envp);
void			exec_child(char **args, char **envp, t_node *node);
void			exec_parents(char **args, char **envp, t_node *node);
void			run_parent_segment(char **args, char **envp, t_node *node);
char			**execute(char **args, char **envp, t_node *node);
void			init_node(t_node *node);
char			**one_commnad(char **args, char **envp, t_node *node);
int				pipe_check(char **args, t_node *node);
int				redir_syntax_check(char **args);
char			**repeat(char **args, char **envp, t_node *node);
char			**split_before_pipe_args(char **args, t_node *node);
void			double_lmove_idx_change(char **args, int *i);
/* is_pipe removed - declared but never defined */
bool			is_ampersand(char *str);
void			cleanup_env(char **envp, t_node *node);

/* redirection */
void			args_left_move(char **args, int i);
void			args_left_move_i(char **args, t_node *node);
void			argu_left_change(char **args, t_node *node);
int				exec_redir(char **args, char **envp, t_node *node);
void			exec_redir_child(char **args, char **envp, t_node *node,
					int *flag);
void			exec_redir_parents(char **args, char **envp, t_node *node,
					int *flag);
bool			is_redir(char **args, int i, int j);
bool			is_redir_check(char *str);
int				left_redir(char **args, char **envp, int *i, t_node *node);
int				left_double_redir(char **args, char **envp, int *i,
					t_node *node);
int				print_err(char **args, int i, t_node *node);
int				print_err2(char **args, int i);
/* print_err3 removed - not used in codebase */
int				redir_chk(char **args);
int				redir_excute(char **args, char **envp, t_node *node, int flag);
int				right_redir(char **args, int *i, t_node *node);
int				right_double_redir(char **args, int *i, t_node *node);
int				heredoc_loop(char **args, char **envp, int *i, t_node *node);
int				setup_heredoc_file(t_node *node);
void			cleanup_heredoc_file(t_node *node);
void			move_redir_args(char **args, char **ori_args, int *i);
bool			is_left_redir(char *str);
bool			is_right_redir(char *str);
bool			is_double_left_redir(char *str);
bool			is_double_right_redir(char *str);
int				left_redir_expand(char **args, int i, t_node *node,
					char **expanded);
void			handle_echo_skip(char **args, t_node *node);
int				open_redir_out(char **args, int i, t_node *node, int flags);
int				left_redir_error_public(const char *s, int type, char *tmp);

/* prompt helpers */
int				handle_escape_char(t_prompt_data *data);
void			handle_pwd_path(char **pwd, char **envp);
char			*expand_loop(char *fmt, char *new_fmt, char *user, char *pwd);
int				promptlen(char *fmt, char **envp, char *pwd, int i);
char			*get_pwd_for_prompt(char **envp, t_node *node);

/* parentheses */
char			**handle_parentheses(char **args, char **envp, t_node *node);

#endif
