" Install vim-plug: https://github.com/junegunn/vim-plug#neovim
" Install coc.nvim: https://github.com/neoclide/coc.nvim
" To enable rustfmt on save:
" :CocConfig
"
" {
"	"coc.preferences.formatOnSaveFiletypes": [
"		"rust"
"	}
" }

call plug#begin()
Plug 'neoclide/coc.nvim', {'branch': 'release'}
Plug 'petRUShka/vim-sage'
Plug 'jacoborus/tender.vim' "colorscheme tender
Plug 'savq/melange'         "colorscheme melange
"Plug 'preservim/vim-markdown'
Plug 'ntpeters/vim-better-whitespace'
call plug#end()

" Make <CR> to accept selected completion item or notify coc.nvim to format
" <C-g>u breaks current undo, please make your own choice
inoremap <silent><expr> <CR> coc#pum#visible() ? coc#pum#confirm()
                              \: "\<C-g>u\<CR>\<c-r>=coc#on_enter()\<CR>"
nmap <silent> gd <Plug>(coc-definition)
nmap <silent> gy <Plug>(coc-type-definition)
nmap <silent> gi <Plug>(coc-implementation)
nmap <silent> gr <Plug>(coc-references)
nnoremap <silent> K :call <SID>show_documentation()<CR>

syntax enable
set termguicolors
colorscheme melange
set cursorline
set cursorcolumn
set number
set relativenumber
"set background=light

set smartindent
set tabstop=4
set shiftwidth=4
set expandtab

let g:better_whitespace_enabled=1
setlocal spell spelllang=en_us
autocmd FileType tex setlocal shiftwidth=2 softtabstop=2 expandtab

command P w | !pdflatex --halt-on-error dp-dap-paper
cabbrev wp P
