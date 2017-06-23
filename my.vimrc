"Go plugins.
" Plug-in manager: https://github.com/junegunn/vim-plug
" Download:
"  curl -fLo ~/.vim/autoload/plug.vim --create-dirs \
"      https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
" Then do:
"  :PlugInstall
"
"Solarized:
"wget https://raw.githubusercontent.com/altercation/vim-colors-solarized/master/colors/solarized.vim
"mkdir -p ~/.vim/colors && mv solarized.vim ~/.vim/colors

call plug#begin('~/.vim/plugged')

Plug 'https://github.com/fatih/vim-go.git'
Plug 'https://github.com/ntpeters/vim-better-whitespace.git'

call plug#end()

syntax on
set background=dark
let g:solarized_termcolors=256
colorscheme solarized
set hlsearch
set autoindent
set history=50
set ruler
set number
set nocompatible
set backupcopy=yes
set viminfo='20,\"50
set tw=80

filetype plugin on
autocmd FileType * set tabstop=2|set shiftwidth=2|set expandtab
autocmd FileType python set tabstop=4|set shiftwidth=4|set expandtab
autocmd FileType go set tabstop=4|set shiftwidth=4|set noexpandtab

command P w | !pdflatex --halt-on-error main
command B w | !bibtex main
cabbrev wp P

set spelllang=en_us spell
