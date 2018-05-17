<template>
    <div class="modal" v-bind:class="{ 'modal-hidden': !isActive, 'modal-shown': isActive }">
        <div class="modal-content">
            <p class="header"><slot name="header"></slot></p>
            <p class="text"><slot name="text"></slot></p>
            <div v-if="buttons && buttons.length > 0" class="buttons">
                <button v-for="button in buttons" v-bind:key="button" v-on:click="click(button)" class="button button-medium button-white">{{button}}</button>
            </div>
        </div>
    </div>
</template>

<script>
export default {
  name: 'Modal',
  data () {
    return {
        isActive: false
    }
  },  
  props: [
    'buttons'
  ],
  methods: {
    open() {
        this.isActive = true;
    },
    click(button) {
        this.isActive = false;
        this.$emit('close', button);
    }
  }
}
</script>

<style scoped>
.modal {
    position: fixed;
    z-index: 10000;
    padding-top: 100px;
    left: 0;
    top: 0;
    width: 100%;
    height: 100%;
    overflow: auto;
    background-color: rgb(0,0,0);
    background-color: rgba(0,0,0,0.4)
}
.modal-hidden {
    display: none;
}
.modal-shown {
    display: block;
}
.modal-content {
    background-color: #fefefe;
    margin: auto;
    padding: 20px;
    border: 1px solid #888;
    width: 80%;
}
p.header {
    font-size: 45px;
    font-weight: bold;
    color: black;
}
p.text {
    font-size: 45px;
    font-weight: bold;
    color: gray;
}
div.buttons {
    margin-top: 80px;
    margin-bottom: 40px;
}
div.buttons button {
    margin-right: 20px;
}
div.buttons button:last-child {
    margin-right: 0;
}
</style>