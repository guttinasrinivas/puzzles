#ifndef __WORD_LIST_IMPL_H__
#define __WORD_LIST_IMPL_H__

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "word_list.h"
#include "utils.h"


struct ManagedWordList {
    using str_p = vas_lib::utils::str_p;
    using int_p = vas_lib::utils::int_p;

    using str_vec_p = vas_lib::utils::str_vec_p;

    std::shared_ptr<std::string> dict_fname_;
    word_list_t* list_;
    int pos_;
    std::shared_ptr<uint8_t> valid_bm_;
    bool wl_loaded_;

    ManagedWordList(std::string& dictfname)
    {
        dict_fname_ = std::make_shared<std::string>(dictfname);
        list_ = new word_list_t;
        /* Find a better way to cast */
        list_->fname = (char*)(dict_fname_->c_str());
        pos_ = -1;
        valid_bm_ = NULL;
        wl_loaded_ = false;
    }

    ~ManagedWordList()
    {
        if (wl_loaded_) {
            wl_cleanup(list_);
            wl_loaded_ = false;
        }
        delete list_;
        list_ = NULL;
    }

    int ReadList(void)
    {
        int ret = wl_read_list(list_);
        if (ret != SUCCESS) {
            throw std::runtime_error("Cannot read the dictionary file");
        }

        wl_loaded_ = true;
        std::shared_ptr<uint8_t> buffer(new uint8_t[list_->n_words],
                                        std::default_delete<uint8_t[]>());
        valid_bm_ = buffer;

        return ret;
    }

    int PrintWords(void)
    {
        for (int ii = 0; ii < list_->n_words; ii++) {
            std::cout << list_->words[ii] << "\n";
        }

        return 0;
    }

    str_vec_p GetWords(void)
    {
        str_vec_p outv = std::make_shared<std::vector<str_p>>();

        for (int ii = 0; ii < list_->n_words; ii++) {
            outv->push_back(make_shared(list_->words[ii]));
        }

        return outv;
    }


    int MapFile(void);

    int CleanUp(void);

    int LookUp(const char* word)
    {
        return wl_lookup(list_, word, &pos_, valid_bm_.get());
    }

    int LookUp(std::string& word)
    {
        return LookUp(word.c_str());
    }

    static auto make(std::string& dictfname)
    {
        return std::make_shared<ManagedWordList>(dictfname);
    }

    word_list_t& operator*(void)
    {
        return *list_;
    }

    str_p make_shared(const char *word)
    {
        return std::make_shared<std::string>(word);
    }
};

using ManagedWordList_p = std::shared_ptr<ManagedWordList>;



#endif /* __WORD_LIST_IMPL_H__ */
