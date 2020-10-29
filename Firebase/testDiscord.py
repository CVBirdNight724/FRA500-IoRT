import discord
from discord.ext import commands
bot = commands.Bot(command_prefix='?>') #กำหนด Prefix
@bot.event
async def on_ready() : #เมื่อระบบพร้อมใช้งาน
    print("Bot Started!") #แสดงผลใน CMD

@bot.event
async def on_message(message) : #ดักรอข้อความใน Chat
    if message.content.startswith('?>ping') : #เมื่อข้อความในตัวแรกมีคำว่า ping
       await message.channel.send('Pong ~ Meow ><') #ข้อความที่ต้องการตอบกลับ
# รันบอท (โดยนำ TOKEN จากบอทที่เราสร้างไว้นำมาวาง)
bot.run('NzM2MTg4NTIzNjkzOTk4MTEw.XxrK_Q.tR6dswq9FavGflIyQSlIig-7IZM')
