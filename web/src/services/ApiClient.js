import axios from "axios";
import Vue from 'vue'

Vue.mixin({
    beforeCreate() {
        const options = this.$options;
        if (options.apiClient)
            this.$apiClient = options.apiClient;
        else if (options.parent && options.parent.$apiClient)
            this.$apiClient = options.parent.$apiClient;
    }
});

export default function makeApiClient(baseUrl) {
    return new ApiClient(baseUrl);
}

export class ApiClient {

    constructor(baseUrl) {
        this.baseUrl = baseUrl;
    }

    deal() {
        return new Promise((resolve, reject) => {
            axios.post(this.baseUrl + '/deal')
                .then(result => {
                    if (result.data.status === 'aiMove') {
                        this.play(result.data)
                            .then(result => {
                                resolve(result);
                            });
                    } else {
                        resolve(result.data);
                    }
                }, error => {
                    reject(error);
                });
        });
    }

    play(state) {
        return new Promise((resolve, reject) => {
            axios.post(this.baseUrl + '/play',
                state,
                {
                    headers: {
                        'Content-Type': 'application/json'
                    }
                })
                .then(result => {
                    resolve(result.data);
                }, error => {
                    reject(error);
                });
        });
    }
}