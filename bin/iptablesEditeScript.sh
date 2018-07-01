#/bin/bash
iptables -F
cat "NetFilter.ini" | while read Line
do
#echo $Line
let lineNum=lineNum+1
ipNull="0.0.0.0"
arr=$(echo $Line|tr "." "\n")
for pro in $arr;
do
echo $pro
if [ $pro -ge 0 ] 2>/dev/null ; then
	echo "lineNum"
	echo $lineNum
	echo "is number"
	ip=$(echo $Line | cut -d \: -f 1)
	port=$(echo $Line | cut -d \: -f 2)
	echo "hello"
	if [ $lineNum -le 700 ] 2>/dev/null ; then
		if [ "$ip" = "0.0.0.0" ]; then
		echo "ip is null"
		iptables -A INPUT -p tcp --sport $port -j ACCEPT
		iptables -A OUTPUT -p tcp --dport $port -j ACCEPT
		else
		echo "ip"
		iptables -A INPUT -p tcp -s $ip --sport $port -j ACCEPT
		iptables -A OUTPUT -p tcp -d $ip --dport $port -j ACCEPT
		fi 
	else
		if [ "$ip" = "0.0.0.0" ]; then
		echo "ip is null"
		iptables -A INPUT -p tcp --dport $port -j ACCEPT
		iptables -A OUTPUT -p tcp --sport $port -j ACCEPT
		else
		echo "ip"
		echo "port"
		echo $port
		iptables -A INPUT -p tcp -s $ip --dport $port -j ACCEPT
		iptables -A OUTPUT -p tcp -d $ip --sport $port -j ACCEPT
		fi 
	fi
	break
else
	let lineNum=lineNum+100
	echo "lineNum"
	echo $lineNum
	echo "is not number"
fi
done
done
iptables -A INPUT -j DROP
iptables -A OUTPUT -j DROP
iptables-save > /etc/init.d/firewall.config
echo "pre-up iptables-restore < /etc/init.d/firewall.config" >> /etc/network/interfaces
echo Fire Wall Config Finished!
 
