<template>
    <div id="console">
      
    </div>
    
</template>

<script>
export default {
  name: 'console',
  methods: {
    created() {
      console.log("wootowoot");
    }
  }
}

</script>

<style scoped>
  #console {
    height: 100%;
  };
</style>
