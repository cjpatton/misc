test -e "${HOME}/.iterm2_shell_integration.zsh" && source "${HOME}/.iterm2_shell_integration.zsh"

PROMPT='%F{green}%*%F:%F{blue}%~%f $ '
export CLICOLOR=1

alias vi="nvim"
export PATH="/usr/local/opt/ruby/bin:$PATH"
export PATH="${PATH}:~/.local/bin"
