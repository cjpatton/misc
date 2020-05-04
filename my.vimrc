"Go plugins.
" Plug-in manager: https://github.com/junegunn/vim-plug
" Download:
"  curl -fLo ~/.vim/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
" Then do:
"  :PlugInstall
"
"Solarized:
"wget https://raw.githubusercontent.com/altercation/vim-colors-solarized/master/colors/solarized.vim
"mkdir -p ~/.vim/colors && mv solarized.vim ~/.vim/colors

call plug#begin('~/.vim/plugged')

Plug 'https://github.com/fatih/vim-go.git'
"Plug 'https://github.com/ntpeters/vim-better-whitespace.git'
Plug 'https://github.com/FStarLang/VimFStar', {'for': 'fstar'}
Plug 'https://github.com/chrisbra/vim-xml-runtime'

call plug#end()


syntax on
set background=dark
let g:solarized_termcolors=256
colorscheme solarized
set hlsearch
set autoindent
set history=50
set ruler
set number relativenumber
set nocompatible
set backupcopy=yes
set viminfo='20,\"50
"set cursorline
"set cursorcolumn
set lazyredraw
set tw=80

autocmd ColorScheme * highlight ExtraWhitespace ctermbg=red guibg=red
"highlight ExtraWhitespace ctermbg=red guibg=red
"match ExtraWhitespace /\s\+$/

highlight ExtraWhitespace ctermbg=red guibg=red
match ExtraWhitespace /\s\+$/
autocmd BufWinEnter * match ExtraWhitespace /\s\+$/
autocmd InsertEnter * match ExtraWhitespace /\s\+\%#\@<!$/
autocmd InsertLeave * match ExtraWhitespace /\s\+$/
autocmd BufWinLeave * call clearmatches()

filetype plugin on
filetype indent off

autocmd FileType * set tabstop=2|set shiftwidth=2|set expandtab
autocmd BufNewFile,BufRead *.py set expandtab shiftwidth=2 softtabstop=2 tabstop=2|
autocmd FileType go set tabstop=4|set shiftwidth=4|set noexpandtab
autocmd BufNewFile,BufRead *.tex set syntax=tex

command P w | !pdflatex --halt-on-error thesis
command B w | !bibtex thesis
cabbrev wp P

setlocal spell spelllang=en_us
hi clear SpellBad
hi SpellBad cterm=underline

au BufRead /tmp/mutt-* set tw=72
