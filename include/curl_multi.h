/* 
 * File:   curl_multi.h
 * Author: Giuseppe
 *
 * Created on March 25, 2014, 11:02 PM
 */

#ifndef curl_multi_H
#define	curl_multi_H

#include "curl_easy.h"

using curl::curl_easy;

namespace curl {
    class curl_multi : public curl_interface<CURLMcode> {
    public:
        class curl_handle;
        explicit curl_multi();
        explicit curl_multi(const long);
        curl_multi(const curl_multi &);
        curl_multi &operator=(const curl_multi &);
        ~curl_multi() noexcept;
        template<typename T> void add(const curl_pair<CURLMoption,T> &);
        template<typename T> void add(const vector<curl_pair<CURLMoption,T>> &);
        template<typename T> void add(const list<curl_pair<CURLMoption,T>> &);
        bool perform();
        const int get_active_transfers() const noexcept;
        const int get_message_queued() const noexcept;
    protected:
        const string to_string(const CURLMcode) const noexcept;
    private:
        int message_queued;
        int active_transfers;
        CURLM *curl;
    };
    
    // Implementation of add method
    template<typename T> void curl_multi::add(const curl_pair<CURLMoption,T> &pair) {
        CURLMcode code = curl_multi_setopt(this->curl,pair.first(),pair.second());
        if (code != CURLM_OK) {
            throw curl_error(this->to_string(code),__FUNCTION__);
        }
    }
    
    // Implementation of add overloaded method
    template<typename T> void curl_multi::add(const vector<curl_pair<CURLMoption,T>> &pairs) {
        for_each(pairs.begin(),pairs.end(),[this](curl_pair<CURLMoption,T> option) {
            this->add(option);
        });
    }
    
    // Implementation of add overloaded method
    template<typename T> void curl_multi::add(const list<curl_pair<CURLMoption,T>> &pairs) {
        for_each(pairs.begin(),pairs.end(),[this](curl_pair<CURLMoption,T> option) {
            this->add(option);
        });
    }
}

#endif	/* curl_multi_H */