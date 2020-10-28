/*eslint-disable*/
export class App extends Component {
    constructor(props) {
        super(props);

        const drawerWidth = 240;

        this.classes = makeStyles((theme) => ({
            root: {
                // flexGrow: 1,
                // display: 'flex',
                // overflow: 'auto',
                height: 400,
                maxWidth: 300,
            },
            appBar: {
                width: `calc(100% - ${drawerWidth}px)`,
                marginLeft: drawerWidth,
            },
            drawer: {
                width: drawerWidth,
                flexShrink: 0,
            },
            drawerPaper: {
                width: drawerWidth,
            },
            content: {
                display: 'flex',
                flexGrow: 1,

                padding: theme.spacing(3),
            },
        }));
        this.inputMessage = React.createRef();
        this.messageListElement = React.createRef();
        this.state = {
            tab: 0,
            ws: null,
            sessionId: getSessionIdFromUrl(),
            user: {},
            typings: new Map(),
            currentChannelId: null,
            currentChannelName: '',
            show: true,
            messageList: [],
            chatList: []
        };
    }

    setUpWebSocket = async (evt) => {
        if (this.state.ws) {
            this.state.ws.close();
            this.setState({
                message: 'Websocket is opened'
            })
        }

        this.state.ws = new WebSocket(getWSURL(this.state.sessionId));
        this.state.ws.onopen = () => {
            console.log('Websocket is opened')
            this.setState({
                message: 'Websocket is opened'
            })
        }
        this.state.ws.onclose = () => {
            console.log('Websocket is closed')
            this.setState({
                message: 'Websocket is closed'
            })
        }
        this.state.ws.onerror = () => {
            console.log('Websocket error')
            this.setState({
                message: 'Websocket error'
            })
        }
        this.state.ws.onmessage = async (evt) => {
            const data = await evt.data.arrayBuffer();
            const incoming = msg.InMessage.deserializeBinary(data);

            switch (incoming.getTypeCase()) {
                case msg.InMessage.TypeCase.CHAT:
                    this.onReceiveChatMessage(incoming.getChat());
                    break;
                case msg.InMessage.TypeCase.SEEN:
                    const seen = incoming.getSeen();
                    console.log('seen' + JSON.stringify(seen.toObject()));
                    break;
                case msg.InMessage.TypeCase.STATUS:
                    const status = incoming.getStatus();
                    console.log('status: ' + JSON.stringify(status.toObject()));
                    break;
                case msg.InMessage.TypeCase.TYPING:
                    const typing = incoming.getTyping();
                    console.log('typing: ' + JSON.stringify(typing.toObject()));
                    break;
                case msg.InMessage.TypeCase.NOTIFICATION:
                    const notification = incoming.getNotification();
                    console.log('notification: ' + JSON.stringify(notification.toObject()));
                    break;
                default:
                    console.log('invalid message: ' + JSON.stringify(incoming.toObject()));
                    break;
            }
        }
    }

    async componentDidMount() {
        let userInfo, chatListData;
        try {
            userInfo = await getUserAuthenInfo(this.state.sessionId);
            chatListData = await getChatList(this.state.sessionId);
            if (userInfo.error || chatListData.error) {
                return;
            }
        } catch (err) {
            return;
        }

        const chatList = await Promise.all(chatListData.data.map(async (data) => {
            return ({
                raw: data,
                channelId: data.channel.id,
                avatar: 'user.ico',
                alt: data.channel.name,
                title: data.channel.name,
                subtitle: getMessageText(data.lastMessage),
                date: data.lastMessage.createAt ? Date.parse(data.lastMessage.createAt) : null,
                unread: 0,
            })
        }));

        // console.log(chatList);
        // console.log(chatListData);
        // console.log(userInfo);

        if (chatListData.error == 0) {
            this.setState({
                user: userInfo.data,
                chatList: chatList
            });
        }

        this.setUpWebSocket();

    }
    componentDidUpdate() {
    }
    scrollToBottom = () => {
        this.messageListElement.current.mlistRef.scrollTop = this.messageListElement.current.mlistRef.scrollHeight;
    }

    sendChatMessage(text) {
        const out = new msg.OutMessage();
        const chat = new msg.Chat();
        const chatContent = new msg.ChatContent();
        chatContent.setContent(text);
        chatContent.setReplyTo(null);
        chatContent.setType(msg.ChatContent.ChatContentType.TEXT);

        chat.setChannelId(this.state.currentChannelId);
        chat.setContent(chatContent);
        chat.setTimestamp(Timestamp.fromDate(new Date()));
        out.setChat(chat);

        this.state.ws.send(out.serializeBinary());
    }

    sendTypingMessage() {
        // let out = new WsOut.WsOut();
        // let typing = new WsOut.Typing();
        // let date = new Timestamp();
        // date.fromDate(new Date());
        // out.setTimestamp(date);
        // out.setTyping(typing);

        // this.state.ws.send(out.serializeBinary());
    }

    createMessage = (chat) => {
        const type = 'text';
        return {
            raw: chat.toObject(),
            notch: false,
            position: (chat.getUserId() == this.state.user.id ? 'right' : 'left'),
            replyButton: true,
            type: type,
            status: 'read',
            theme: (chat.getUserId() == this.state.user.id ? 'white' : 'dark'),
            text: chat.getContent().getContent(),
            date: chat.getTimestamp().toDate(),
            avatar: `user.ico`,
        };
    }

    onReceiveChatMessage = (chat) => {
        this.addMessage(chat);
        this.setState((oldState) => {
            let chatList = oldState.chatList;
            const idx = chatList.findIndex((chatItem) => chatItem.channelId == chat.getChannelId())
            if (idx != -1) {
                chatList[idx].subtitle = chat.getContent().getContent();
                chatList[idx].date = chat.getTimestamp().toDate();
                return ({
                    chatList: chatList
                })
            }

        })
    }

    addMessage(chat) {
        let list = this.state.messageList;
        list.push(this.createMessage(chat));
        this.setState({
            messageList: list,
        });
        this.scrollToBottom()
    }

    handleTyping = debounce(() => {
        console.log('500ms')
    }, 100);

    onChatListClick = async (chat) => {
        const channelId = chat.raw.channel.id;
        const channelName = chat.raw.channel.name;

        const messageList = await getMessageList(this.state.sessionId, channelId);

        this.setState((oldState) => ({
            currentChannelId: channelId,
            currentChannelName: channelName,
            messageList: messageList.data.map(
                (msg) => ({
                    raw: msg,
                    notch: false,
                    position: oldState.user.id == msg.userId ? 'right' : 'left',
                    replyButton: true,
                    type: 'text',
                    status: 'read',
                    theme: (oldState.user.id == msg.userId ? 'white' : 'dark'),
                    text: msg.content.content,
                    date: Date.parse(msg.createAt),
                    avatar: 'user.ico',
                })
            )
        }))
    }

    render() {
        return (
            <div className={this.classes.root}>
            {
                console.log(this.classes)
            }
                <List disablePadding style={{ width: '100%', overflow: 'auto' }}>
                    {
                        [1, 2, 3, 4, 5].map((x) => (<ListItem button>
                            <ListItemText>absdf</ListItemText>
                        </ListItem>))
                    }
                    {[1, 2, 3, 4, 5].map((x) => (<ListItem button>
                        <ListItemText>absdf</ListItemText>
                    </ListItem>))}
                    {[1, 2, 3, 4, 5].map((x) => (<ListItem button>
                        <ListItemText>absdf</ListItemText>
                    </ListItem>))}
                    {[1, 2, 3, 4, 5].map((x) => (<ListItem button>
                        <ListItemText>absdf</ListItemText>
                    </ListItem>))}

                    <ListItem>
                        <ListItemText>absdf</ListItemText>
                    </ListItem>
                    <ListItem>
                        <ListItemText>absdf</ListItemText>
                    </ListItem>
                </List>
            </div>
        )
        return (
            <React.Fragment className={this.classes.root}>
                <CssBaseline />
                <Drawer
                    className={this.classes.drawer}
                    variant="permanent"
                    anchor="left"
                    classes={{
                        paper: this.classes.drawerPaper,
                    }}
                >
                    <AppBar position="static" color="default" className={this.classes.appBar}>
                        <Tabs
                            value={this.state.tab}
                            onChange={(evt, newValue) => { this.setState({ tab: newValue }) }}
                            variant="fullWidth"
                            indicatorColor="primary"
                            textColor="primary"
                            aria-label="tabs"
                            maxWidth="sm"
                        >
                            <Tab icon={<ChatIcon />} aria-label="chat" label="chat" {...a11yProps(0)} />
                            <Tab icon={<PeopleIcon />} aria-label="connect" label="connect" {...a11yProps(1)} />
                            <Tab icon={<MonetizationOnIcon />} aria-label="money" label="money" {...a11yProps(2)} />
                        </Tabs>
                    </AppBar>
                    <TabPanel value={this.state.tab} index={0}>
                        <List>
                            <ListItem>
                                <Fab color="primary" aria-label="add" onClick={() => { console.log("add clicked") }}>
                                    <AddIcon />
                                </Fab>
                            </ListItem>
                            <Divider />
                            <ListItem >
                                <ChatList
                                    className="full-size"
                                    dataSource={this.state.chatList}
                                    onClick={this.onChatListClick}
                                />
                            </ListItem>
                        </List>
                    </TabPanel>
                    <TabPanel value={this.state.tab} index={1}>
                        Item Two
                    </TabPanel>
                    <TabPanel value={this.state.tab} index={2}>
                        Item Three
                    </TabPanel>
                </Drawer>

                <Container className={this.classes.content} component="main" maxWidth="md">
                    <div className={this.classes.toolbar} />
                    <List style={{ overflow: 'auto' }}>
                        {
                            [1, 2, 3, 4, 5].map((x) => (<ListItem button>
                                <ListItemText>absdf</ListItemText>
                            </ListItem>))
                        }
                        {[1, 2, 3, 4, 5].map((x) => (<ListItem button>
                            <ListItemText>absdf</ListItemText>
                        </ListItem>))}
                        {[1, 2, 3, 4, 5].map((x) => (<ListItem button>
                            <ListItemText>absdf</ListItemText>
                        </ListItem>))}
                        {[1, 2, 3, 4, 5].map((x) => (<ListItem button>
                            <ListItemText>absdf</ListItemText>
                        </ListItem>))}

                        <ListItem>
                            <ListItemText>absdf</ListItemText>
                        </ListItem>
                        <ListItem>
                            <ListItemText>absdf</ListItemText>
                        </ListItem>
                    </List>
                    <Typography paragraph>
                        Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt
                        ut labore et dolore magna aliqua. Rhoncus dolor purus non enim praesent elementum
                        facilisis leo vel. Risus at ultrices mi tempus imperdiet. Semper risus in hendrerit
                        gravida rutrum quisque non tellus. Convallis convallis tellus id interdum velit laoreet id
                        donec ultrices. Odio morbi quis commodo odio aenean sed adipiscing. Amet nisl suscipit
                        adipiscing bibendum est ultricies integer quis. Cursus euismod quis viverra nibh cras.
                        Metus vulputate eu scelerisque felis imperdiet proin fermentum leo. Mauris commodo quis
                        imperdiet massa tincidunt. Cras tincidunt lobortis feugiat vivamus at augue. At augue eget
                        arcu dictum varius duis at consectetur lorem. Velit sed ullamcorper morbi tincidunt. Lorem
                        donec massa sapien faucibus et molestie ac.
                    </Typography>
                    <Typography paragraph>
                        Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt
                        ut labore et dolore magna aliqua. Rhoncus dolor purus non enim praesent elementum
                        facilisis leo vel. Risus at ultrices mi tempus imperdiet. Semper risus in hendrerit
                        gravida rutrum quisque non tellus. Convallis convallis tellus id interdum velit laoreet id
                        donec ultrices. Odio morbi quis commodo odio aenean sed adipiscing. Amet nisl suscipit
                        adipiscing bibendum est ultricies integer quis. Cursus euismod quis viverra nibh cras.
                        Metus vulputate eu scelerisque felis imperdiet proin fermentum leo. Mauris commodo quis
                        imperdiet massa tincidunt. Cras tincidunt lobortis feugiat vivamus at augue. At augue eget
                        arcu dictum varius duis at consectetur lorem. Velit sed ullamcorper morbi tincidunt. Lorem
                        donec massa sapien faucibus et molestie ac.
                    </Typography>

                    <Typography paragraph>
                        {
                            [1, 2, 3, 4, 5, 6, 7, 8].map(x => (<div>{[1, 2, 3, 4, 5, 6, 7, 8].map(x => (<div>x</div>))}</div>))
                        }
                    </Typography>
                </Container>


            </React.Fragment>
        )
        return (

            <div className='container'>
                <AppBar position="static" color="default">
                    <Tabs
                        value={this.state.tab}
                        onChange={(evt, newValue) => { this.setState({ tab: newValue }) }}
                        variant="fullWidth"
                        indicatorColor="primary"
                        textColor="primary"
                        aria-label="tabs"
                    >
                        <Tab icon={<ChatIcon />} aria-label="chat" label="chat" {...a11yProps(0)} />
                        <Tab icon={<PeopleIcon />} aria-label="connect" label="connect" {...a11yProps(1)} />
                        <Tab icon={<MonetizationOnIcon />} aria-label="money" label="money" {...a11yProps(2)} />
                    </Tabs>
                </AppBar>
                <TabPanel value={this.state.tab} index={0}>
                    <div className='chat-list'>
                        <ChatList
                            dataSource={this.state.chatList}
                            onClick={this.onChatListClick}
                        />
                    </div>
                </TabPanel>
                <TabPanel value={this.state.tab} index={1}>
                    Item Two
                        </TabPanel>
                <TabPanel value={this.state.tab} index={2}>
                    Item Three
                        </TabPanel>
                {/*<Paper square className='tab'> </Paper>*/}

                <div className='right-panel'>
                    <div className="channel-name">
                        <h2>{this.state.currentChannelName}</h2>
                    </div>

                    <MessageList
                        ref={this.messageListElement}
                        className='message-list'
                        // lockable={true}
                        toBottomHeight={'100%'}
                        // downButtonBadge={10}
                        dataSource={this.state.messageList} />
                    <Input
                        placeholder="Enter message."
                        defaultValue=""
                        ref={this.inputMessage}
                        multiline={true}
                        className='input-chat'
                        onKeyPress={(e) => {
                            if (e.shiftKey && e.charCode === 13) {
                                return true;
                            }

                            if (e.charCode === 13) {
                                this.sendChatMessage(e.target.value);
                                this.inputMessage.current.clear();
                                e.preventDefault();
                                return false;
                            }
                            // this.handleTyping()
                            // this.sendTypingMessage();
                        }}
                        rightButtons={
                            <Button
                                text='Send'
                                onClick={this.sendChatMessage.bind(this)}
                            />
                        } />
                </div>
            </div>
        );
    }
}

export default App;
