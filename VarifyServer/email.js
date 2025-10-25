const nodemailer = require('nodemailer')
const config_module = require('./config')

//创建发送邮件代理
let transport = nodemailer.createTransport({
    host:'smtp.qq.com',
    port:465,
    secure:true,
    auth:{
        user:config_module.email_user,    //发送方邮件地址
        pass:config_module.email_pass     //邮件授权码或密码
    }

})

//发送邮件    使用回调函数机制，达到异步操作
function SendMail(mailOptions_){
    return new Promise(function(resolve,reject){
        transport.sendMail(mailOptions_,function(error,info){
            if(error){
                console.log(error);
                reject(error);
            }
            else{
                console.log('邮件已成功发送'+info.response);
                resolve(info.response);
            }
        });
    })
}

module.exports.SendMail = SendMail;