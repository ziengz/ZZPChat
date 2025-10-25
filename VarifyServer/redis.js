const config_module = require('./config')
const Redis = require("ioredis")

//创建redis客户端实例
const RedisCli = new Redis({
    host: config_module.redis_host,
    port: config_module.redis_port,
    password: config_module.redis_passwd,
})

//监听错误信息
RedisCli.on("error",function(err){
    console.log("RedisCli connect error");
    RedisCli.quit();
});

//根据key获取value

async function GetRedis(key){
    try{
        const result = await RedisCli.get(key)
        if(result === null){
            console.log('result:','<'+result+'>','This key cannot be find...')
            return null;
        }
        console.log('Result:','<'+result+'>','Get key success!...')
        return result;
    }catch(error){
        console.log('GetRedis error is',error);
        return null;
    }
}

//根据key查询redis中是否存在key
async function QueryRedis(key){
    try{
        const result = await RedisCli.exists(key)
        //判断该值是否为空，如果为空返回null
        if(result === 0){
            console.log('result:','<'+'>','This key is null...');
            return null;
        }
        console.log('Result:','<'+result+'>','With this value!...');
        return result;
    }catch(error){
        console.log('Queryredis error is',error);
        return null;
    }
}

//设置key和value，并过期时间
async function SetRedisExpire(key,value,Exptime){
    try{
        //设置键和值
        await RedisCli.set(key,value)
        //设置过期时间（以秒为单位）
        await RedisCli.expire(key,Exptime);
        return true;

    }catch(error){
        console.log('SetRedisExpire error is ',error);
        return false;
    }
}

function Quit(){
    RedisCli.quit();
}

module.exports = {GetRedis,SetRedisExpire,Quit,QueryRedis}