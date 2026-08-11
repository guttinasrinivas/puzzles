set colorcolumn=110
highlight ColorColumn ctermbg=darkblue
augroup project
  autocmd!
  autocmd BufRead,BufNewFile *.h,*.c set filetype=c.doxygen
augroup END
nnoremap <leader>b :!bash build_all.sh<CR>:!cscope -Rb<CR>:cs reset<CR><CR>
